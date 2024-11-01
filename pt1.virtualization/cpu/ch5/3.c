#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork faild\n");
  } else if (rc == 0) {
    printf("hello");
  } else {
		sleep(1);
    printf("goodbye");
  }
}
