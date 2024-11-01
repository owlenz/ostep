#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
  char balls[] = "balls";
  int lol = PRIO_PROCESS;
  printf("%s", balls);
  id_t pid = getpid();
  setpriority(lol, pid, -20);
  int nice = getpriority(lol, pid);
  printf("%d\n", nice);
}
