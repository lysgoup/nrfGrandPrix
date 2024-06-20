#ifndef BUZZER_H
#define BUZZER_H
#include <zephyr/drivers/pwm.h>

#define BUZZER_NODE DT_ALIAS(pwm_led0)

#if DT_NODE_HAS_STATUS(BUZZER_NODE, okay)
#define PWM_CTLR    DT_PWMS_CTLR(BUZZER_NODE)
#define PWM_CHANNEL DT_PWMS_CHANNEL(BUZZER_NODE)
#define PWM_FLAGS   DT_PWMS_FLAGS(BUZZER_NODE)
#else
#error "Unsupported board: buzzer_pwm devicetree alias is not defined"
#endif

static const struct device *pwm = DEVICE_DT_GET(PWM_CTLR);
static const struct pwm_dt_spec pwm_spec =  PWM_DT_SPEC_GET(BUZZER_NODE);

void init_buzzer();
void buzzer_on(uint32_t hz, uint32_t time);
void buzzer_off(uint32_t time);
#endif