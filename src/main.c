#include <zephyr.h>
#include <device.h>
#include <drivers/pwm.h>

#define PWM_LABEL DT_LABEL(DT_NODELABEL(pwm0))
#define PWM_CHANNEL 0 // PWM 채널 번호
#define PERIOD_USEC (USEC_PER_SEC / 1000U) // PWM 주기: 1kHz (1ms)

void main(void) {
    const struct device *pwm_dev;
    uint32_t pulse_width;
    int ret;

    pwm_dev = device_get_binding(PWM_LABEL);
    if (!pwm_dev) {
        printk("Error: PWM device %s not found\n", PWM_LABEL);
        return;
    }

    // 50% Duty Cycle 설정
    pulse_width = PERIOD_USEC / 2U;

    while (1) {
        // PWM 신호 출력
        ret = pwm_pin_set_usec(pwm_dev, PWM_CHANNEL, PERIOD_USEC, pulse_width, 0);
        if (ret) {
            printk("Error %d: failed to set pulse width\n", ret);
            return;
        }

        k_sleep(K_MSEC(1000)); // 1초 동안 소리 내기

        // PWM 신호 끄기
        ret = pwm_pin_set_usec(pwm_dev, PWM_CHANNEL, PERIOD_USEC, 0, 0);
        if (ret) {
            printk("Error %d: failed to stop PWM\n", ret);
            return;
        }

        k_sleep(K_MSEC(1000)); // 1초 동안 소리 끄기
    }
}
