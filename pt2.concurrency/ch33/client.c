#include <arpa/inet.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_SECS 10

int main() {
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd == -1) {
    perror("cannot open client socket");
    return 1;
  }
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(4040);
  if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) != 1) {
    perror("error converting ip str to byte");
    return 1;
  }

  // Establishing Connection
  for (int i = 0; i < MAX_SECS; i++) {
    if (connect(client_fd, (const struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {
      perror("error connecting with server");
      sleep(1);
    } else {
      printf("Client: connection established\n");
      break;
    }
    return 1;
  }
  ssize_t n;
  struct timeval tv;
  // receiving time of day
  while ((n = recv(client_fd, &tv, sizeof(tv), 0)) > 0) {
    printf("Client: Data received: seconds: %ld\n", tv.tv_sec);
  }
  if (n < 0)
    perror("recv errro");
}
