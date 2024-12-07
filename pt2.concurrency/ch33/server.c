#include <arpa/inet.h>
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

int main() {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    perror("error opening socket");
    return 1;
  }
  struct in_addr in_addr;
  inet_pton(AF_INET, "127.0.0.1", &in_addr);

  struct sockaddr_in *sockaddr = malloc(sizeof(struct sockaddr_in));
  memset(sockaddr, 0, sizeof(struct sockaddr_in));
  sockaddr->sin_family = AF_INET;
  sockaddr->sin_addr = in_addr;
  sockaddr->sin_port = htons(4040);

  if (bind(socket_fd, (const struct sockaddr *)sockaddr,
           sizeof(struct sockaddr_in)) == -1) {
    perror("bind failed");
    close(socket_fd);
    return 1;
  }

  if (listen(socket_fd, 5) == -1) {
    perror("listen failed");
    return 1;
  }

  printf("Server: Socket successfully bound to 127.0.0.1:4040\n");
  struct pollfd fds[1];
  fds[0].fd = socket_fd;
  fds[0].events = POLLIN;
  while (1) {

		// Polling
    int poll_result = poll(fds, 1, -1);
    if (poll_result == -1)
      perror("poll error");
    
		socklen_t sockaddr_size = sizeof(struct sockaddr_in);
    
		if (fds[0].revents & POLLIN) {
      int client_fd = accept(fds[0].fd, NULL, NULL);
      if (client_fd == -1) {
        perror("error with client fd");
        return 1;
      } else {
        printf("Server: got a connection\n");
      }
      struct timeval tv;
      gettimeofday(&tv, NULL);
      if (send(client_fd, &tv, sizeof(tv), MSG_CONFIRM) == -1) {
        perror("error sending time of day");
        return 1;
      } else {
        printf("Server: time of day sent\n");
      }
    }
  }

  close(socket_fd);
}
