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

void put(int val) {
  arr[put_ptr] = val;
  put_ptr = (put_ptr + 1) % MAX;
}

int get() {
  int val = arr[get_ptr];
  get_ptr = (get_ptr + 1) % MAX;
  return val;
}

void *producer(void *x) {
  for (int i = 0; i < LOOPS; i++) {
    sem_wait(&empty); // Wait for an empty slot

    sem_wait(&mutex);
    // Produce item
    /*printf("Producer %ld: Produced item %d\n", (long)pthread_self(), i);*/
    printf("produce in 1 sec thread %d\n", *(int *)x);
    put(i * *(int *)x);
    sleep(1);
    sem_post(&mutex);

    sem_post(&full); // Signal that a slot is full
  }
  return NULL;
}

void *concumer() {
  for (int i = 0; i < LOOPS; i++) {
    sem_wait(&full);
    sem_wait(&mutex);
    printf("concume %d\n", get());
    sem_post(&mutex);
    sem_post(&empty);
  }
  return NULL;
}

int main() {
  sem_init(&empty, 0, MAX);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);
  pthread_t a, b, c, d;
  int x = 0, y = 1;
  pthread_create(&a, NULL, concumer, NULL);
  pthread_create(&b, NULL, concumer, NULL);
  pthread_create(&c, NULL, producer, (void *)&x);
  pthread_create(&d, NULL, producer, (void *)&y);
  pthread_join(a, NULL);
  pthread_join(b, NULL);
  pthread_join(c, NULL);
  pthread_join(d, NULL);
}
