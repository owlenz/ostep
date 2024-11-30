#include <unistd.h> // This is just for file descriptor constants (1 for stdout)

void _start() {
  int x = 10;
  asm("mov $1, %%rax;"
      "mov $1, %%rdi;"
      "mov %0, %%rsi;"
      "mov $4, %%rdx;"
      "syscall;"
      :
      : "r"(x)
      : "%rax", "%rdi", "rsi", "rdx");
  asm("mov $60, %%rax;" // syscall number for sys_exit
      "xor %%rdi, %%rdi;"
      "syscall;"
      :
      :
      : "%rax", "%rdi");
}
