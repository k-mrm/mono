#include "aarch64.h"
#include "spinlock.h"
#include "printk.h"

bool holding(struct spinlock *lk) {
  return lk->locked && lk->cpuid == cpuid();
}

void acquire(struct spinlock *lk) {
  disable_irq();

  if(holding(lk))
    panic("already held %d", lk->cpuid);

  /* TODO: don't depend on gcc builtin-function */
  while(__sync_lock_test_and_set(&lk->locked, 1) != 0)
    ;

  isb();

  lk->cpuid = cpuid();
}

void release(struct spinlock *lk) {
  if(!holding(lk))
    panic("invalid release");

  lk->cpuid = -1;

  isb();

  asm volatile("str wzr, %0" : "=m"(lk->locked));

  enable_irq();
}

void lock_init(struct spinlock *lk) {
  lk->locked = 0;
  lk->cpuid = -1;
}