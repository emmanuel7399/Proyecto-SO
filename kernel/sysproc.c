#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;
  struct proc *p = myproc();

  argint(0, &n);

  addr = p->sz;

  // MODIFICACION: Asignacion perezosa (Lazy Allocation)
  // Ya no llamamos a growproc(n). En su lugar, simplemente
  // incrementamos el tamaño virtual del proceso.
  p->sz = p->sz + n;

  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
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

// MODIFICACION: Implementacion de settickets
// Toma un argumento entero (n) del espacio de usuario y se lo asigna
// a la variable tickets del proceso actual.
uint64
sys_settickets(void)
{
  int n;
  struct proc *p = myproc();

  // En xv6-riscv, argint es de tipo void y lee el registro directamente
  argint(0, &n);

  // Un proceso siempre debe tener al menos 1 tiquete para evitar inanicion
  if(n < 1)
    return -1;

  // Adquirimos el candado para modificar el estado del proceso de forma segura
  acquire(&p->lock);
  p->tickets = n;
  release(&p->lock);

  return 0; // Exito
}
