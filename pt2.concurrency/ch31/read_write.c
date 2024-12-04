#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#define MAX 3
#define LOOPS 10

sem_t empty;
sem_t full;
sem_t mutex;
int arr[MAX];
int put_ptr = 0;
int get_ptr = 0;

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

void _write(int val) {
  arr[put_ptr] = val;
  put_ptr = (put_ptr + 1) % MAX;
}

int _read() {
  int val = arr[get_ptr];
  return val;
}

void *producer(void *x) {
  for (int i = 0; i < LOOPS; i++) {
    rwlock_wait_write();
    printf("write from thread, %d\n", *(int *)x);
    _write(i * *(int *)x);
    rwlock_post_write();
  }
  return NULL;
}

void *concumer() {
  for (int i = 0; i < LOOPS; i++) {
    rwlock_wait_read();
    printf("read, %d\n", _read());
    rwlock_post_read();
  }
  return NULL;
}

int main() {
  sem_init(&empty, 0, MAX);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);
	rwlock_init(&rw);
  pthread_t a, b, c, d;
  int x = 1, y = 2;
  pthread_create(&a, NULL, concumer, NULL);
  pthread_create(&b, NULL, concumer, NULL);
  pthread_create(&c, NULL, producer, (void *)&x);
  pthread_create(&d, NULL, producer, (void *)&y);
  pthread_join(a, NULL);
  pthread_join(b, NULL);
  pthread_join(c, NULL);
  pthread_join(d, NULL);
}
