#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int file = open("./balls.md", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork faild\n");
  } else if (rc == 0) {
    char *text = "#balls";
    printf("hello im child pid:(%d)\n", (int)getpid());
    int z = write(file, text, strlen(text));
    sleep(3);
  } else {
    char *text = "##balls";
    int z = write(file, text, strlen(text));

    printf("i am parent pid:(%d) of child pid:(%d) %d\n", (int)getpid(), rc, 1);
    int rc_wait = wait(NULL);
  }
}
