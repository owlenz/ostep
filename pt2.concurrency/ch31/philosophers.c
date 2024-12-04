#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#define MAX 5
#define LOOPS 10

int arr[MAX];
sem_t forks[MAX];

typedef struct rwlock {
  sem_t lock;
  sem_t write_lock;
  int readers;
} rwlock_t;

rwlock_t rw;

void rwlock_init(rwlock_t *lock) {
  sem_init(&lock->lock, 0, 1);
  sem_init(&lock->write_lock, 0, 1);
  lock->readers = 0;
}

void rwlock_wait_read() {
  sem_wait(&rw.lock);
  rw.readers++;
  if (rw.readers == 1)
    sem_wait(&rw.write_lock);
  sem_post(&rw.lock);
}

void rwlock_post_read() {
  sem_wait(&rw.lock);
  rw.readers--;
  if (rw.readers == 0)
    sem_post(&rw.write_lock);
  sem_post(&rw.lock);
}

void rwlock_wait_write() { sem_wait(&rw.write_lock); }

void rwlock_post_write() { sem_post(&rw.write_lock); }

void eat(int p) {
  printf("philosopher %d is eating\n", p);
  sleep(1);
}

void think(int p) {
  printf("philosopher %d is thinking\n", p);
  sleep(3);
}

int left(int p) { return p; };
int right(int p) { return (p + 1) % MAX; };

void get_forks(int p) {
  int left_f = left(p);
  int right_f = right(p);
  if (p == MAX - 1) {

    sem_wait(&forks[right_f]);
    sem_wait(&forks[left_f]);
  } else {

    sem_wait(&forks[left_f]);
    sem_wait(&forks[right_f]);
  }
}

void put_forks(int p) {
  int left_f = left(p);
  int right_f = right(p);
  sem_post(&forks[left_f]);
  sem_post(&forks[right_f]);
}

void *action(void *x) {
  while (1) {
    think(*(int *)x);
    get_forks(*(int *)x);
    eat(*(int *)x);
    put_forks(*(int *)x);
  }
  return NULL;
}

int main() {
  for (int i = 0; i < MAX; i++) {
    sem_init(&forks[i], 0, 1);
  }
  pthread_t a, b, c, d, e;
  int x = 0, y = 1, z = 2, j = 3, k = 4;
  pthread_create(&a, NULL, action, (void *)&x);
  pthread_create(&b, NULL, action, (void *)&y);
  pthread_create(&c, NULL, action, (void *)&z);
  pthread_create(&d, NULL, action, (void *)&j);
  pthread_create(&e, NULL, action, (void *)&k);
  pthread_join(a, NULL);
  pthread_join(b, NULL);
  pthread_join(c, NULL);
  pthread_join(d, NULL);
  pthread_join(e, NULL);
}
