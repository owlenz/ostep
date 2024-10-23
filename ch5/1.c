#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork faild\n");
  } else if (rc == 0) {
    printf("hello im child pid:(%d)\n", (int)getpid());
    sleep(3);
    printf("hello im child pid:(%d)\n", (int)getpid());
  } else {
    printf("i am parent pid:(%d) of child pid:(%d) %d\n", (int)getpid(), rc, 1);
  }
}
