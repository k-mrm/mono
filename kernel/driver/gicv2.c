#include "mono.h"
#include "memmap.h"
#include "printk.h"
#include "driver/gicv2.h"

#define GICD_BASE (GICV2_BASE)
#define GICC_BASE (GICV2_BASE + 0x10000)

#define GICD_CTLR (GICD_BASE)
#define GICD_TYPER  (GICD_BASE + 0x4)
#define GICD_ISENABLER(n) (GICD_BASE + 0x100 + (u64)(n) * 4)
#define GICD_IPRIORITYR(n)  (GICD_BASE + 0x400 + (u64)(n) * 4)
#define GICD_ITARGETSR(n)  (GICD_BASE + 0x800 + (u64)(n) * 4)

#define GICC_CTLR (GICC_BASE)
#define GICC_PMR  (GICC_BASE + 0x4)
#define GICC_IAR  (GICC_BASE + 0xc)
#define GICC_EOIR (GICC_BASE + 0x10)
#define GICC_HPPIR  (GICC_BASE + 0x18)
#define GICC_AIAR (GICC_BASE + 0x20)
#define GICC_AEOIR  (GICC_BASE + 0x24)

void gicv2_init() {
  printk("gicv2 init\n");

  REG(GICD_CTLR) = 0;

  u32 typer = REG(GICD_TYPER);
  u32 max_nint = ((typer & 0x1f) + 1) * 32;

  /* FIXME */
  u32 archtimer_id = 27; /* arch timer intid: 27 */

  REG(GICD_ISENABLER(archtimer_id / 32)) |= 1 << (archtimer_id % 32);

  REG(GICD_IPRIORITYR(archtimer_id / 4)) &= ~((u32)0xff << (archtimer_id % 4 * 8));

  u32 target = 0;
  u32 itargetsr = REG(GICD_ITARGETSR(archtimer_id / 4));
  itargetsr &= ~((u32)0xff << (archtimer_id % 4 * 8));
  REG(GICD_ITARGETSR(archtimer_id / 4)) = itargetsr | ((u32)(1 << target) << (archtimer_id % 4 * 8));

  REG(GICC_CTLR) = 0x1;
  REG(GICD_CTLR) = 0x1;
}

bool gic_enabled() {
  return (REG(GICC_CTLR) & 0x1) && (REG(GICD_CTLR) & 0x1);
}

void gic_eoi(u32 iar) {
  REG(GICC_EOIR) = iar;
}

u32 gic_iar() {
  return REG(GICC_IAR);
}