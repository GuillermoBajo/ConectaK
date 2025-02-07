#include <inttypes.h>

#include "timer0_driver.h"
#include "fifo.h"
#include "gpio.h"
#include "alarmas.h"
#include "botones.h"
#include "watchdog.h"

#ifndef TEST_H
#define TEST_H

int test_all(void);
void test_watchdog(void);

#endif
