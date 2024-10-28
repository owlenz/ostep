#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 1000

int main() {
  int x = 100;
  int filedes[2];
  int status = pipe(filedes);
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork faild\n");
  } else if (rc == 0) {
    close(filedes[0]);
    dup2(filedes[1], STDOUT_FILENO);
    close(filedes[1]);
    printf("balls\n");
    exit(EXIT_SUCCESS);
  }

  int rc1 = fork();

  if (rc1 < 0) {
    fprintf(stderr, "fork faild\n");

  } else if (rc1 == 0) {
    close(filedes[1]);
    wait(NULL);

    dup2(filedes[0], STDIN_FILENO);
    close(filedes[0]);
    char input[SIZE];
    scanf("%s", input);

    printf("Received from pipe: %s \n", input);
    close(filedes[0]);
		exit(EXIT_SUCCESS);
  } 
	wait(NULL);
}
