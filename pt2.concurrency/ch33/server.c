#include <aio.h>
#include <arpa/inet.h>
#include <bits/types/struct_timeval.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define MAX_CLIENTS 10

int set_nonblocking(int fd) {
  int erm = fcntl(fd, F_GETFL, 0);
  if (erm == -1)
    return -1;
  return fcntl(fd, F_SETFL, erm | O_NONBLOCK);
}

int main() {
  // setting up socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("error opening socket");
    return 1;
  }

  // setting up address
  struct in_addr in_addr;
  inet_pton(AF_INET, "127.0.0.1", &in_addr);

  struct sockaddr_in *sockaddr = malloc(sizeof(struct sockaddr_in));
  memset(sockaddr, 0, sizeof(struct sockaddr_in));
  sockaddr->sin_family = AF_INET;
  sockaddr->sin_addr = in_addr;
  sockaddr->sin_port = htons(4040);

  // binding socket with address
  if (bind(server_fd, (const struct sockaddr *)sockaddr,
           sizeof(struct sockaddr_in)) == -1) {
    perror("bind failed");
    close(server_fd);
    return 1;
  }

  if (listen(server_fd, 10) == -1) {
    perror("listen failed");
    return 1;
  }

  if (set_nonblocking(server_fd) == -1) {
    perror("Failed to set non-blocking");
    return 1;
  }

  printf("Server: Socket successfully bound to 127.0.0.1:4040\n");
  struct pollfd fds[MAX_CLIENTS + 1];
  fds[0].fd = server_fd;
  fds[0].events = POLLIN;

  for (int i = 1; i < MAX_CLIENTS + 1; i++) {
    fds[i].fd = -1;
    fds[i].events = POLLIN;
  }

  struct aiocb *aiocbp = malloc(MAX_CLIENTS * sizeof(struct aiocb));
  for (int i = 0; i < MAX_CLIENTS; i++) {
    memset(&aiocbp[i], 0, sizeof(struct aiocb));
    aiocbp[i].aio_buf = malloc(BUF_SIZE);
    aiocbp[i].aio_nbytes = BUF_SIZE;
    aiocbp[i].aio_fildes = 0;
  }

  while (1) {

    // Polling
    int poll_result = poll(fds, MAX_CLIENTS + 1, 100);

    if (poll_result == -1)
      perror("poll error");

    if (fds[0].revents & POLLIN) {
      int client_fd = accept(fds[0].fd, NULL, NULL);
      if (client_fd == -1) {
        perror("error with client fd");
        return 1;
      } else {
        printf("Server: got a connection\n");
      }
      int full = 1;
      for (int i = 1; i < MAX_CLIENTS + 1; i++) {
        if (fds[i].fd == -1) {
          fds[i].fd = client_fd;
          fds[i].events = POLLIN;
          aiocbp[i - 1].aio_fildes = client_fd;
          aio_read(&aiocbp[i - 1]);
          full = 0;
          break;
        }
      }
      if (full) {
        printf("Max client reached");
        close(client_fd);
      }
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (aiocbp[i].aio_fildes > 0) {
        int err_res = aio_error(&aiocbp[i]);
        if (err_res == EINPROGRESS) {
          aiocbp[i].aio_sigevent.sigev_value.sival_int++;
        } else if (err_res == 0) {
          /*printf("Read successful for fd %d after %d iter, sec:%ld\n",*/
          /*       aiocbp[i].aio_fildes,*/
          /*       aiocbp[i].aio_sigevent.sigev_value.sival_int,*/
          printf("MESSAGE: %s\n", (char *)aiocbp[i].aio_buf);
          close(aiocbp[i].aio_fildes);
          aiocbp[i].aio_fildes = -1;
        } else if (err_res != 0) {
          perror("aio_error");
        }
      }
    }
  }

  close(server_fd);
}
