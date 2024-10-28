#include <stdio.h>
#include <stdlib.h>

#define NAME_MAX 10

typedef struct process {
  char name[NAME_MAX];
  int stride;
  int pass;
} process;

//

process remove_min(process **all, int *size) {
  int min = 0;
  for (int i = 0; i < 3; i++) {
    if (all[min]->pass > all[i]->pass)
      min = i;
  }

  process min_proc = *all[min];

  for (int i = min; i < *size - 1; i++) {
    all[i] = all[i + 1];
  }

  (*size)--;

  return min_proc;
}

void schedule(process proc) {
  printf("process %s scheduled (pass: %d)\n", proc.name, proc.pass);
}

int main() {

  process A = {"A", 100, 0};
  process B = {"B", 200, 0};
  process C = {"C", 40, 0};
  process *all[] = {&A, &B, &C};
  int size = sizeof(all) / sizeof(all[0]);

  int i = 0;
  while (i < 20) {
    process curr = remove_min(all, &size);

    process *new_proc = malloc(sizeof(process));
    *new_proc = curr;
    new_proc->pass += new_proc->stride;
    schedule(*new_proc);
    all[2] = new_proc;
    size++;
    i++;
  }
}
