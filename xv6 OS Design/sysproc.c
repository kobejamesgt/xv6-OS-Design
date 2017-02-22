#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_start_burst(void)
{
  int i;
  proc->burst_counts = (proc->burst_counts + 1) % 11;
  i = proc->burst_counts;
  proc->cpu_start_bursts[i] = sys_uptime();
  //cprintf("start %d %d\n", i, proc->cpu_start_bursts[i]);
  return 0;
}

int
sys_end_burst(void)
{
  int i;
  i = proc->burst_counts;
  proc->cpu_end_bursts[i] = sys_uptime();
  //cprintf("end %d %d\n", i, proc->cpu_end_bursts[i]);
  proc->cpu_bursts[i] = proc->cpu_end_bursts[i] - proc->cpu_start_bursts[i];
  return 0;
}

int
sys_print_bursts(void)
{
  int i;
  for (i = 0; i < 10; i++)
  {
    cprintf("%d ", proc->cpu_bursts[i]);
  }
  cprintf(", turnaround time: %d\n", proc->cpu_end_bursts[9] - proc->cpu_start_bursts[0]);
  return 0;
}

int 
sys_thread_create(void)
{
  int tmain, stack, arg; 
  if(argint(0, &tmain) < 0 || argint(1, &stack) || argint(2, &arg))
    return -1;
  return thread_create((void*)tmain, (void*)stack, (void*) arg); 
}

int
sys_thread_join(void)
{
  int stack;
  if(argint(0, &stack)<0)
    return -1;
  return thread_join((void **)stack);
}

int 
sys_mtx_create(void)
{
  int locked;
  if(argint(0, &locked) < 0)
    return -1;
  return mtx_create(locked);
}

int 
sys_mtx_lock(void)
{
  int lock_id;
  if(argint(0, &lock_id) < 0)
    return -1;
  return mtx_lock(lock_id);
}

int
sys_mtx_unlock(void)
{
  int lock_id;
  if(argint(0, &lock_id) < 0)
    return -1;
  return mtx_unlock(lock_id);
}