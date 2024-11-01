/*
 * Vector has all its content (arr) in a contigous block in memory. this could
 * be bad because if realloc doesnt find enough space for new content it will
 * move the arr into new location.
 *
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct vector {
  int size;
  int cap;
  int *arr;
} vector;

vector *new_vector() {
  vector *vec = (vector *)malloc(sizeof(vector));
  vec->size = 0;
  vec->cap = 1;
  vec->arr = malloc(sizeof(int) * vec->cap);
  return vec;
}

void add(int x, vector *vec) {

  if (vec->size >= vec->cap) {
    vec->cap *= 2;
    vec->arr = realloc(vec->arr, sizeof(int) * vec->cap);
  }

  vec->arr[vec->size] = x;
  vec->size++;
  return;
}

void print(const vector *vec) {
  for (int i = 0; i < vec->size; i++) {
    printf("%d ", vec->arr[i]);
  }
  printf("\n");
}

int main() {
  vector *vec = new_vector();
  add(10, vec);
  add(20, vec);
  add(30, vec);
  add(40, vec);
  add(50, vec);
  free(vec->arr);
  free(vec);
  return 0;
}
