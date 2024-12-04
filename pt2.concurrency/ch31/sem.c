
#include <pthread.h>
typedef struct sema_t {
  int val;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} sema_t;

void sema_init(sema_t *sem, int val) {
  sem->val = val;
  pthread_mutex_init(&sem->mutex, 0);
  pthread_cond_init(&sem->cond, 0);
};

void sema_wait(sema_t *sem) {
  pthread_mutex_lock(&sem->mutex);
  while (sem->val < 0)
    pthread_cond_wait(&sem->cond, &sem->mutex);
  sem->val--;
  pthread_mutex_unlock(&sem->mutex);
}

void sema_post(sema_t *sem) {
  pthread_mutex_lock(&sem->mutex);
  sem->val++;
  pthread_cond_signal(&sem->cond);
  pthread_mutex_unlock(&sem->mutex);
}
