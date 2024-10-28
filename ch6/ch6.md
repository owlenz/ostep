# Trap Handling
1. Syscall in the running process
2. cpu stores reg, PC,IP,flags,cs(code segment) at the kernel stack
3. cpu switches context and move to kernel mode
4. cpu looks into the trap vector table (instruction descriptor table) to find the specific trap handler then jumps into it.
5. Kernel handles the trap (doing the syscall or whatever the trap was)
6. Kernel returns from trap (IRET)
7. cpu restore regs,IP,etc.. from kernel stack to user-space stack
8. cpu moves to user mode and jumps to the PC (program counter) after the trap (in the initial regs)
9. process (code) continues executing
