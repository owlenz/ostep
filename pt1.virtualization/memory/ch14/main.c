#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
  char *dynamicStr = malloc(50 * sizeof(char)); // Allocate 20 characters
  if (dynamicStr == NULL) {
    perror("Failed to allocate memory");
    return 1;
  }

  // Copy a string into the allocated space
  strcpy(dynamicStr, "Hello, World!");

  // Print the string and its length
  printf("String: %s\n", dynamicStr);
  printf("Length: %zu\n", strlen(dynamicStr));
	free(dynamicStr);
	return 0;
}
