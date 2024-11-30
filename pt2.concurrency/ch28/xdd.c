#define _GNU_SOURCE // Required for CPU_SET and sched_setaffinity

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "./include/xdd.h"


int main() {
  int i = 0;
  set_affinity_to_core(3);
  while (i++ != 1) {
    int b4 = clock();
    pthread_t a, b, c;
    lock_init(&xdd);
    pthread_create(&a, NULL, worker, (void *)(intptr_t)1);
    pthread_create(&b, NULL, worker, (void *)(intptr_t)2);
    pthread_create(&c, NULL, worker, (void *)(intptr_t)3);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    pthread_join(c, NULL);
    int after = clock();
    printf("gg:%d | in:%ld \n", gg, (after - b4) / CLOCKS_PER_SEC);
  }
}
