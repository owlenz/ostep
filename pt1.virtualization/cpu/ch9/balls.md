# CFS

- vruntime of a process determines whether to run it (prioritize it over other processes or not)
    - vruntime increases over every time slice run by the process 
    - the lower the niceness (higher weighting) of a process the less vruntime accumulated each time slice 
        (as the os wants it to run more)

- time slice of a process is determined its weight, total weight of processes and the sched_latency
    - time_slice = weight(k)/Sum(weight) * sched_latency
- process in CFS are stored in rbt
- when a process goes to sleep for a period of time (it will have lower vruntime than all other process) if not handled 
    correctly it will monopolize the cpu. So, CFS handles this by altering the vruntime of the process to the lowest 
    vruntime in the tree.

