#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
  int x = 100;
  pid_t rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork faild\n");
  } else if (rc == 0) {
    printf("hello\n");
    /*int rcW = wait(NULL);*/
    /*printf("wait = %d ", rcW);*/
		sleep(4);
  } else {
    int rcW = waitpid(rc, NULL, 0);
    /*int rcW = wait(NULL);*/
    printf("wait = %d ", rcW);
  }
}
