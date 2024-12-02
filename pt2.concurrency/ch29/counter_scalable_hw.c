#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <time.h>

#define VALUE 6000000

typedef struct {
  int global_c;
  int local_c[4];
  pthread_mutex_t local_lock[4];
  pthread_mutex_t global_lock;
  int threshold;
} counter_t;

counter_t c;

void init(counter_t *c) {
  pthread_mutex_init(&c->global_lock, NULL);
  for (int i = 0; i < 4; i++) {
    pthread_mutex_init(&c->local_lock[i], NULL);
    c->local_c[i] = 0;
  }
  c->global_c = 0;
  c->threshold = 1024;
}

void inc(counter_t *c) {
  int cpu = sched_getcpu();
  pthread_mutex_lock(&c->local_lock[cpu]);
  c->local_c[cpu]++;
  if (c->local_c[cpu] >= c->threshold) {
    pthread_mutex_lock(&c->global_lock);
    c->global_c += c->local_c[cpu];
    pthread_mutex_unlock(&c->global_lock);
    c->local_c[cpu] = 0;
  }
  pthread_mutex_unlock(&c->local_lock[cpu]);
}

void *worker() {
  for (int i = 0; i < VALUE; i++) {
    inc(&c);
  }
  return NULL;
}

int main() {
  struct timespec ts;
  pthread_t x, y, z,a,b;
  init(&c);
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
    perror("Failed to get time");
    return 1;
  }
  pthread_create(&x, NULL, worker, NULL);
  pthread_create(&y, NULL, worker, NULL);
  pthread_create(&z, NULL, worker, NULL);
  pthread_create(&a, NULL, worker, NULL);
  pthread_create(&b, NULL, worker, NULL);
  pthread_join(x, NULL);
  pthread_join(y, NULL);
  pthread_join(z, NULL);
  pthread_join(a, NULL);
  pthread_join(b, NULL);

  struct timespec ts1;
  if (clock_gettime(CLOCK_MONOTONIC, &ts1) != 0) {
    perror("Failed to get time");
    return 1;
  }
  long sec = ts1.tv_sec - ts.tv_sec;
  long nsec = ts1.tv_nsec - ts.tv_nsec;
  if (nsec < 0) {
    sec -= 1;
    nsec += 1000000000L;
  }
  printf("counter: %d, time: %ld.%09ld", c.global_c, sec, nsec);
}
