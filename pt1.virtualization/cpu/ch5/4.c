#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork faild\n");
  } else if (rc == 0) {
    char *balls[3] = {"ls", "..",NULL};
    char *path = getenv("PATH");
		execl(path, "ls", balls[1]);
    printf("hello");
  } else {
    printf("goodbye");
  }
}
