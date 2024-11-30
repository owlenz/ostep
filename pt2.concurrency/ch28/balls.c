#define _GNU_SOURCE // Required for CPU_SET and sched_setaffinity

#include "./include/xdd.h"
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>


void lock_init(lock_t *lock) {
  lock->gturn = 0;
  lock->xturn = 0;
}

int fetch_and_add(int *ptr) {
  int value = 1;
  __asm__ volatile("lock; xaddl %0, %1"
                   : "+r"(value), "+m"(*ptr) // input + output
                   :                         // No input-only
                   : "memory");
  return value;
}

void lock(lock_t *lock) {
  int myturn = fetch_and_add(&lock->xturn);
  while (myturn != lock->gturn)
    sched_yield();
}

void unlock(lock_t *lock) {
  __asm__ volatile("" ::: "memory");
  lock->gturn++;
}

void *worker(void *num) {
  int numi = (int)(intptr_t)num;
  printf("thread num:%d \n", numi);
  printf("ticket num:%d \n", xdd.xturn);
  for (int i = 0; i < 10000000; i++) {
    lock(&xdd);
    gg++;
    /*printf("gg:%d \n", gg);*/
    unlock(&xdd);
  }
  return NULL;
}

void set_affinity_to_core(int core_id) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  // Apply affinity to the current thread
  pthread_t thread = pthread_self();
  if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0) {
    perror("pthread_setaffinity_np");
  }
}
