#include "balls.h"
#include <aio.h>
#include <arpa/inet.h>
#include <bits/types/struct_timeval.h>
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

#define MAX_CLIENTS 10

// my dumb ass didnt know that setting this as non-blocking
// will affect make recv non-blocking too xddMORS
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
  while (1) {

    // Polling
    int poll_result = poll(fds, MAX_CLIENTS + 1, 1000);

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
          full = 0;
          break;
        }
      }
      if (full) {
        printf("Max client reached");
        close(client_fd);
      }
    }
    for (int i = 1; i < MAX_CLIENTS + 1; i++) {
      if (fds[i].revents & POLLIN && fds[i].fd != -1) {
        void *buf = malloc(sizeof(struct message));
        int n = recv(fds[i].fd, buf, BUF_SIZE, 0);
        if (n == -1) {
          perror("SERVER: error recieving");
          continue;
        } else if (n == 0) {
          perror("SERVER: client disconnected");
          close(fds[i].fd);
          fds[i].fd = -1;
          continue;
        } else {
          struct message *msg = (struct message *)buf;
          if (msg->type == PATH) {
            int file = open(msg->content, O_RDONLY, S_IRWXU);
            if (file == -1) {
              perror("Error opening file");
            }
            char buf[BUF_SIZE];
            read(file, buf, BUF_SIZE);
            msg->type = STRING;
            strcpy(msg->content, buf);
            if ((send(fds[i].fd, msg, sizeof(struct message), MSG_CONFIRM)) ==
                -1) {
              perror("error sending file content");
              return 1;
            } else {
              printf("Server: file content sent \n");
              close(fds[i].fd);
              fds[i].fd = -1;
            }
          }
        }
      }
    }
  }

  close(server_fd);
}
