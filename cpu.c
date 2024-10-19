#include <assert.h>
#include <features.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define LOWER 0
#define UPPER 300
#define STEP 300

int balls;

float F_to_C(float *F) {
  scanf("%f", F);
  float C = (5.0 / 9.0) * (*F - 32.0);

  printf("F:%.0f\tC:%.2f\n", *F, C);
  return C;
}

void print_80plus_char() {
  int ln, c;
  char *line = "balls";

  for (ln = 0; (c = getchar()) != '\n' && c != EOF; ++ln) {
    line[ln] = c;
  }

  if (ln >= 80) {
    printf("%s", line);
  }
}

void clean_lines() {
  int ln, c;
  char line[1000];

  for (ln = 0; (c = getchar()) != '\n' && c != EOF; ++ln) {
    line[ln] = c;
  }

  line[ln] = '\0';

  for (; ln > 0 && (line[ln - 1] == '\t' || line[ln - 1] == ' '); --ln) {
  }

  line[ln] = '\0';

  printf("%s %d", line, ln);
}

void reverse(char s[]) {
  int length = strlen(s);

  for (int i = length - 1; i >= 0; --i) {
    printf("%c", s[i]);
  }
}

int main(int argc, char *argv[]) {

  /*float C = F_to_C(&F);*/
  /*print_80plus_char();*/
  /*clean_lines();*/
  /*reverse("ballsxddMORS");*/

  void *firstEnd = sbrk(0);

  /*sbrk(0x1);*/

  int *newArr = (int *)firstEnd;

  printf("%p %p %p", &newArr[0], &newArr[1], &newArr[30]);

  return 0;
}
