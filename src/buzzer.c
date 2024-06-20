#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>

#include "./include/buzzer.h"

void init_buzzer()
{
    printk("PWM-based buzzer control\n");

    if (!device_is_ready(pwm)) {
        printk("Error: PWM device %s is not ready\n", pwm->name);
        return;
    }
}

void buzzer_on(uint32_t hz, uint32_t time){
  int ret = pwm_set_dt(&pwm_spec, (uint32_t)(1/(float)hz*1000000000), 150000);
  if (ret < 0) {
      printk("Error %d: failed to set pulse width\n", ret);
      return;
  }

  k_sleep(K_MSEC(time));
}

void buzzer_off(uint32_t time){
  // Turn off the buzzer
  int ret = pwm_set_dt(&pwm_spec, PWM_HZ(1000), 0);
  if (ret < 0) {
      printk("Error %d: failed to set pulse width\n", ret);
      return;
  }
  k_sleep(K_MSEC(time));
}