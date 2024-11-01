#include <stdio.h>
#include <stdlib.h>

int main() {
  int *arr = (int *)malloc(100 * sizeof(int));
	arr[100] = 10;
  printf("%d", arr[100]);
	return 0;
}
