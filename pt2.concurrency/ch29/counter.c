#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <time.h>

typedef struct {
  int global;
  int local[4];
  pthread_mutex_t glock;
  pthread_mutex_t llock[4];
  int threshold;
} counter_t;

counter_t c;

void init(int threshold, counter_t *c) {
  c->threshold = threshold;
  pthread_mutex_init(&c->glock, NULL);
  for (int i = 0; i < 4; i++) {
    pthread_mutex_init(&c->llock[i], NULL);
    c->local[i] = 0;
  }
  c->global = 0;
}

void inc(counter_t *c) {
  int cpu = sched_getcpu();
  pthread_mutex_lock(&c->llock[cpu]);
  c->local[cpu]++;
  if (c->local[cpu] == c->threshold) {
    pthread_mutex_lock(&c->glock);
    c->global += c->local[cpu];
    pthread_mutex_unlock(&c->glock);
    c->local[cpu] = 0;
  }
  pthread_mutex_unlock(&c->llock[cpu]);
};

int get(counter_t *c) {
  pthread_mutex_lock(&c->glock);
  int x = c->global;
  pthread_mutex_unlock(&c->glock);
  return x;
}

void *worker() {
  int i = 10000000;
  while (i--) {
    inc(&c);
  }
  printf("%d\n", get(&c));
  return NULL;
}

int main() {
  pthread_t x, y, z;
  init(5, &c);
  pthread_create(&x, NULL, worker, NULL);
  pthread_create(&y, NULL, worker, NULL);
  pthread_create(&z, NULL, worker, NULL);
  pthread_join(x, NULL);
  pthread_join(y, NULL);
  pthread_join(z, NULL);

  printf("%d\n", get(&c));
}
