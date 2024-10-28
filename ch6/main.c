#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void signal_handler(int sig) {
  printf("Process being forcefully interrupted");
  exit(0);
}

int main() {
  signal(SIGINT, signal_handler);
  clock_t start = clock();
  for (int i = 0; i < 900;i++) {
    /*sched_yield();*/
    printf("%d\n", i);
  }
  clock_t end = clock();
  double cpu_time_used =
      ((double)(end - start)) / CLOCKS_PER_SEC;

  printf("Time taken: %f seconds\n", cpu_time_used);
}
