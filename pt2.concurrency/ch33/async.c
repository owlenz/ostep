#include <aio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 100

int main() {
  int file = open("./input.txt", O_RDONLY, S_IRWXU);
  if (file == -1) {
    perror("balls error open file");
    return -1;
  }
  char *str = malloc(BUFF_SIZE);
  if (!str) {
    perror("error allocating string");
    return 1;
  }
  struct aiocb *aiocbp = malloc(sizeof(struct aiocb));
  if (!aiocbp) {
    perror("Error allocating aiocb");
    free(str);
    close(file);
    return 1;
  }
  memset(aiocbp, 0, sizeof(struct aiocb));
  aiocbp->aio_fildes = file;
  aiocbp->aio_buf = str;
  aiocbp->aio_nbytes = BUFF_SIZE;
  aiocbp->aio_offset = 0;

  if (aio_read(aiocbp) == -1) {
    free(str);
    free(aiocbp);
    return -1;
  }
  while (aio_error(aiocbp) == EINPROGRESS) {
    // wait
  }
  if (aio_error(aiocbp) == -1) {
    free(str);
    free(aiocbp);
    return -1;
  }

  printf("buf output: %s \n", (char *)aiocbp->aio_buf);
  int fdx = open("output.txt", O_WRONLY, S_IRWXU);
  if (fdx == -1) {
    perror("balls error open file");
    return -1;
  }
  int wr = write(fdx, (const void *)aiocbp->aio_buf,
                 strlen((const char *)aiocbp->aio_buf));
  if (wr == -1) {
    perror("balls error write to output");
    return -1;
  }
}
