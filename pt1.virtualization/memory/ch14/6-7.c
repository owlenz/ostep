#include <stdio.h>
#include <stdlib.h>

int main() {
  int *arr = (int *)malloc(100 * sizeof(int));
  printf("%d\n", arr[10]);
	free(&arr[10]);
  printf("%d\n", arr[10]);
	return 0;
}
