#include <signal.h>
#include <stdio.h>
#include <unistd.h>
void handler() { printf("Don't stop me now\n"); }

int main() {
  printf("process id (pid): %d\n", getpid());
  signal(SIGHUP, handler);
  while (1)
    ;
}
