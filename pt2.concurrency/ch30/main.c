#include <pthread.h>
#include <semaphore.h>

typedef struct {
  pthread_mutex_t *lock;
  pthread_cond_t *cond;
  int done;
} signal;

int main() {  

}
