#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define VALUE 10000000

typedef struct {
  int count;
  pthread_mutex_t mutex;
} counter_t;

counter_t c;

void init(counter_t *c) {
  pthread_mutex_init(&c->mutex, NULL);
  c->count = 0;
}

void inc(counter_t *c) {
  pthread_mutex_lock(&c->mutex);
  c->count++;
  pthread_mutex_unlock(&c->mutex);
}

void *worker() {
  int i = VALUE;
  while (i--) {
    inc(&c);
  }
  return NULL;
}

int main() {
  struct timespec ts;
  pthread_t x, y, z;
  init(&c);
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
    perror("Failed to get end time");
    return 1;
  }
  pthread_create(&x, NULL, worker, NULL);
  pthread_create(&y, NULL, worker, NULL);
  pthread_create(&z, NULL, worker, NULL);
  pthread_join(x, NULL);
  pthread_join(y, NULL);
  pthread_join(z, NULL);
  struct timespec ts1;
  if (clock_gettime(CLOCK_MONOTONIC, &ts1) != 0) {
    perror("Failed to get end time");
    return 1;
  }
  long sec = ts1.tv_sec - ts.tv_sec;
  long nsec = ts1.tv_nsec - ts.tv_nsec;
  if (nsec < 0) {
    sec -= 1;
    nsec += 1000000000L;
  }
  printf("counter: %d, time: %ld.%09ld", c.count, sec, nsec);
}
