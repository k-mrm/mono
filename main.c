#include "gpio.h"

int main(void) {
  REG(GPFSEL4) = 0x01 << 21;
  REG(GPSET0) = 0x01 << (47-32);

  for(;;) {
    ;
  }
  return 0;
}
