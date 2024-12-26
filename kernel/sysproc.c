#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  uint64 start_va;
  int num_pages;
  uint64 user_mask;

  argaddr(0, &start_va);
  argint(1, &num_pages);
  argaddr(2, &user_mask);

  if (num_pages > 64)
  {
    return -1; // Limit the number of pages to 64
  }

  uint64 mask = 0;
  uint64 va0, va1;
  pte_t *pte;
  va0 = PGROUNDDOWN(start_va);
  pagetable_t pagetable = myproc()->pagetable;

  for (int i = 0; i < num_pages; i++)
  {
    va1 = va0 + i * PGSIZE;
    if ((pte = walk(pagetable, va1, 1)) == 0)
      return -1;
    if ((*pte & PTE_A) == PTE_A)
    { // the A flag is 1
      // update mask at position i is 1
      mask = mask | (1 << i);
      // reset A flag to 0
      *pte = *pte & ~(1L << 6);
    }
  }
  //copy mask in kernel to user space and check if error
  if (copyout(pagetable, user_mask, (char *)&mask, sizeof(mask)) < 0)
  {
    return -1;
  }
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
