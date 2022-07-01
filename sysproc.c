#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
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
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
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
sys_getHelloWorld(void)
{
  return getHelloWorld();
}

int
sys_getTicks(void)
{
  return ticks;
}

int
sys_getProcInfo(void)
{
  return getProcInfo();
}

int 
sys_thread_create(void)
{
  int stack;

  if(argint(0, &stack) < 0)
    return -1;
    
  return thread_fork((void*)stack);
}

int 
sys_thread_id(void)
{
  struct proc * curproc = myproc();
  if(curproc->main_thread == 1)
    return -1;
  return curproc->pid;
}

int 
sys_thread_join(void)
{
  int tid;

  if(argint(0, &tid) < 0)
    return -1;

  return thread_join(tid);
}

int
sys_set_execution_priority()
{
  int priority;
  if (argint(0, &priority) < 0)
    return -1;

  return set_execution_priority(priority);
}

int
sys_get_sched_policy()
{
  return get_sched_policy();
}

int
sys_set_sched_policy()
{
  int policy;
  if (argint(0, &policy) < 0)
    return -1;

  return set_sched_policy(policy);
}

int sys_wait_and_fill_statistics(void)
{
  int *turnAroundtime, *waitingtime, *cbttime, *pario;
  if (argptr(0, (void*)&turnAroundtime, sizeof(turnAroundtime)) < 0 ||
      argptr(1, (void*)&waitingtime, sizeof(waitingtime)) < 0 ||
      argptr(2, (void*)&cbttime, sizeof(cbttime)) < 0 ||
      argptr(3, (void*)&pario, sizeof(pario)) < 0)
    return -1;
  return wait_and_fill_statistics(turnAroundtime, waitingtime, cbttime, pario);
}