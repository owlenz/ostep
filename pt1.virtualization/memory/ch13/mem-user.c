#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEGA 1024 * 1024

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "no arguments passed");
  }
  int megabytes = atoi(argv[1]);
  int duration = (argc == 3) ? atoi(argv[2]) : -1;
  int *arr = malloc(MEGA * atoi(argv[1]));

  time_t start = time(NULL);

  while (1) {
    for (int i = 0; i < (megabytes * MEGA) / sizeof(int); i++) {
      arr[i] = i;
      printf("%d\n",arr[i]);
    }
    time_t current = time(NULL);
    if (duration > 0 && (current - start) >= duration)
      break;
  }

  free(arr);
  printf("Memory usage completed.\n");
}
