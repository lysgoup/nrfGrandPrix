#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include "./include/led.h"
#include "./include/joy.h"

#define STACK_SIZE 1024
#define PRIORITY 7

K_THREAD_STACK_DEFINE(joystick_stack, STACK_SIZE);
struct k_thread joystick_thread_data;
static bool stop_thread = false;


// 조이스틱 관련 전역 변수
static const int ADC_MAX = 1023;
static const int MIDDLE = ADC_MAX / 2;
int32_t nowX = 0, nowY = 0;

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
    !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
    ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
                         DT_SPEC_AND_COMMA)
};

void read_adc(const struct adc_dt_spec *adc_channel, int32_t *value)
{
    uint16_t buf;
    struct adc_sequence sequence = {
        .buffer = &buf,
        .buffer_size = sizeof(buf),
    };
    (void)adc_sequence_init_dt(adc_channel, &sequence);
    int err = adc_read(adc_channel->dev, &sequence);
    if (err < 0) {
        printk("Could not read (%d)\n", err);
    } else {
        *value = (int32_t)buf;
    }
}

void joystick_thread(void *arg1, void *arg2, void *arg3)
{
    while (!stop_thread) {
        //read_adc(&adc_channels[0], &nowX);
        read_adc(&adc_channels[1], &nowY);

        if(nowY == ADC_MAX) nowY = MIDDLE;
        else if(nowY > 65500) nowY = 0;

        //printk("Joy X: %" PRIu32 ", ", nowX);
        printk("Joy Y: %" PRIu32 "\n", nowY);

        k_sleep(K_MSEC(100)); // 주기적으로 조이스틱 값을 읽어들임
    }
}

void start_joystick_thread(void)
{   
    stop_thread = false;
    k_thread_create(&joystick_thread_data, joystick_stack,
                    K_THREAD_STACK_SIZEOF(joystick_stack),
                    joystick_thread,
                    NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
}

void stop_joystick_thread(void){
    stop_thread = true;
    k_thread_join(&joystick_thread_data, K_FOREVER);
}

int joyCheckMove(void) // current checking for only y axis
{
    if (nowY > MIDDLE) {
        return UP; 
    } else if (nowY < MIDDLE) {
        return DOWN;
    }
    return STAY; // 움직임 없음
}

int joystick_init(void)
{
    int err;

    // ADC 채널 초기화
    for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
        if (!adc_is_ready_dt(&adc_channels[i])) {
            printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
            return -1;
        }

        err = adc_channel_setup_dt(&adc_channels[i]);
        if (err < 0) {
            printk("Could not setup channel #%d (%d)\n", i, err);
            return -1;
        }
    }
    return 0;
}
