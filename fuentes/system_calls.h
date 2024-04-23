#include <LPC210X.H>    // LPC21XX Peripheral Registers
#include <stdint.h>

#ifndef SYSTEM_CALLS_H
#define SYSTEM_CALLS_H

uint32_t __swi(1) read_IRQ_bit(void);

void __swi(0XFF) enable_irq(void);
void __swi(0xFE) disable_irq(void);
void __swi(0xFD) disable_fiq(void);


uint32_t system_calls_test(void);

#endif
