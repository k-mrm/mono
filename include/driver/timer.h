#ifndef AARCH64_OS_DRIVER_TIMER_H
#define AARCH64_OS_DRIVER_TIMER_H

#include "kernel.h"

void timer_init(u64 interval_ms);

void timer_init_per_cpu(void);

#endif
