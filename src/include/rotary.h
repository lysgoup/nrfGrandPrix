#ifndef ROTARY_H
#define ROTARY_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define STACK_SIZE 1024
#define PRIORITY 7

extern void start_rotary_thread(void);
extern void stop_rotary_thread(void);
extern int rotary_init(void);

#endif // ROTARY_H
