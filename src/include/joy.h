#ifndef JOY_H
#define JOY_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>

// 외부에서 접근 가능한 전역 변수
extern const struct adc_dt_spec adc_channels[];

bool isChange(void);
void read_adc(const struct adc_dt_spec *adc_channel, int32_t *value);
void joystick_thread(void *arg1, void *arg2, void *arg3);
void start_joystick_thread(void);
void stop_joystick_thread(void);
int joyCheckMove(void);
int joystick_init(void);

#endif // JOY_H
