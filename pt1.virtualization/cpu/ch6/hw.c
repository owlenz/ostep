#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define BUFFERSIZE 10

int main() {
  int fd = open("./read", O_RDONLY);
  char buff[10];
  clock_t time_b4 = clock();
  clock_t gettime_b4 = clock_gettime();
  printf("time b4 %ld\n", time_b4);

  /*for (int i = 0; i < 1000000; i++) {*/
  /*  read(fd, buff, 10);*/
  /*}*/

  clock_t time_after = clock();

  double time_taken = ((double)(time_after - time_b4)) / CLOCKS_PER_SEC;
  printf("Time Taken for 1000000 syscall: %f Second\n", time_taken);
  close(fd);
}
