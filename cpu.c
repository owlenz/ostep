#include <assert.h>
#include <fcntl.h>
#include <features.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

long long loops;
pthread_mutex_t lock;

void *worker(void *balls) {
  for (long long i = 0; i < loops; i++) {
    pthread_mutex_lock(&lock);
    int *ball = (int *)balls;
    *ball += 1;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

int main(int argc, char *argv[]) {

  loops = atoi(argv[1]);

  long long balls = 0;

  pthread_t p1, p2;

  pthread_mutex_init(&lock, NULL);
  pthread_create(&p1, NULL, worker, (void *)&balls);
  pthread_create(&p2, NULL, worker, (void *)&balls);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);

  int fd = open("./balls.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

  assert(fd > -1);
  printf("balls");

  int rc = write(fd, "balls balls\t balllsda\nvagabond\0", 30);

  assert(rc == 30 && "balls");

  close(fd);

  printf("%lld", balls);

  pthread_mutex_destroy(&lock);

  return 0;
}
