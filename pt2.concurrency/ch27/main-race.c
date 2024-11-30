#include <stdio.h>

#include "common-threads.h"

int balance = 0;
pthread_mutex_t m;
void *worker(void *arg) {
  printf("this should print first\n");
  Pthread_mutex_lock(&m);
  balance++;
  Pthread_mutex_unlock(&m);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t p;
  Pthread_mutex_init(&m, NULL);
  Pthread_create(&p, NULL, worker, NULL);
  Pthread_mutex_lock(&m);
  balance++;
  Pthread_mutex_unlock(&m);
  Pthread_join(p, NULL);
  printf("this should print last\n");
  return 0;
}
