#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/printk.h>

#define BUZZER_NODE DT_ALIAS(pwm_led0)

#if DT_NODE_HAS_STATUS(BUZZER_NODE, okay)
#define PWM_CTLR    DT_PWMS_CTLR(BUZZER_NODE)
#define PWM_CHANNEL DT_PWMS_CHANNEL(BUZZER_NODE)
#define PWM_FLAGS   DT_PWMS_FLAGS(BUZZER_NODE)
#else
#error "Unsupported board: buzzer_pwm devicetree alias is not defined"
#endif

void main(void)
{
    const struct device *pwm;
    int ret;

    printk("PWM-based buzzer control\n");

    pwm = DEVICE_DT_GET(PWM_CTLR);
    if (!device_is_ready(pwm)) {
        printk("Error: PWM device %s is not ready\n", pwm->name);
        return;
    }

    const struct pwm_dt_spec pwm_spec = {
        .dev = pwm,
        .channel = PWM_CHANNEL,
        .flags = PWM_FLAGS,
    };
    // const struct pwm_dt_spec sBuzzer = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

    while (1) {
        printk("check\n");
        // Set PWM to 1kHz with 50% duty cycle
        ret = pwm_set_dt(&pwm_spec, (uint32_t)(1/(float)932*1000000000), 150000);
        if (ret < 0) {
            printk("Error %d: failed to set pulse width\n", ret);
            return;
        }

        // k_sleep(K_MSEC(1000));

        // Turn off the buzzer
        // ret = pwm_set_dt(&sBuzzer, PWM_HZ(1000), 0);
        // if (ret < 0) {
        //     printk("Error %d: failed to set pulse width\n", ret);
        //     return;
        // }

        // k_sleep(K_MSEC(1000));
    }
}


// #include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/pwm.h>
// #include <zephyr/sys/printk.h>

// #define PWM_LED0_NODE DT_ALIAS(buzzer_pwm)

// #if DT_NODE_HAS_STATUS(PWM_LED0_NODE, okay)
// #define PWM_CTLR	DT_PWMS_CTLR(PWM_LED0_NODE)
// #define PWM_CHANNEL	DT_PWMS_CHANNEL(PWM_LED0_NODE)
// #define PWM_FLAGS	DT_PWMS_FLAGS(PWM_LED0_NODE)
// #else
// #error "Unsupported board: buzzer_pwm devicetree alias is not defined"
// #endif

// #define PERIOD_USEC	(1000000U / 880U) // 주파수 880Hz
// #define DUTY_CYCLE	(PERIOD_USEC / 2U) // 듀티 사이클 50%

// int main(void)
// {
// 	const struct device *pwm;
// 	int ret;

// 	printk("PWM-based buzzer control\n");

// 	pwm = DEVICE_DT_GET(PWM_CTLR);
// 	if (!device_is_ready(pwm)) {
// 		printk("Error: PWM device %s is not ready\n", pwm->name);
// 		return 1;
// 	}

// 	while (1) {
// 		ret = pwm_set_dt(pwm, PWM_HZ(1000), PWM_HZ(1000) / 2);
// 		if (ret) {
// 			printk("Error %d: failed to set pulse width\n", ret);
// 			return 1;
// 		}

// 		k_sleep(K_MSEC(1000));

// 		// 부저 끄기
// 		ret = pwm_pin_set_usec(pwm, PWM_CHANNEL, PERIOD_USEC, 0, PWM_FLAGS);
// 		if (ret) {
// 			printk("Error %d: failed to set pulse width\n", ret);
// 			return 1;
// 		}

// 		k_sleep(K_MSEC(1000));
// 	}
//     return 0;
// }
