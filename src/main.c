#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <inttypes.h>

#define ADC_BUFFER_SIZE 256 // 버퍼 크기, 필요에 따라 조정 가능
#define SENSOR_INVALID_VALUE 65535 // ADC 값의 유효 범위를 벗어나는 값으로 설정
#define MAX_SENSORVALUE 1023 // 10비트 ADC의 최대 값
#define MIN_SENSORVALUE 0 // ADC의 최소 값

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static int16_t buf[ADC_BUFFER_SIZE]; // ADC 값을 저장할 버퍼

struct adc_sequence sequence = {
        .buffer = &buf,
        .buffer_size = sizeof(buf),
};

static const struct adc_dt_spec adc_channels[] = {
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
};

int main(void)
{
        printk("start!!\n");
        for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
                if (!adc_is_ready_dt(&adc_channels[i])) {
                        printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
                        return 0;
                }

                int err = adc_channel_setup_dt(&adc_channels[i]);
                if (err < 0) {
                        printk("Could not setup channel #%d (%d)\n", i, err);
                        return 0;
                }
        }

        while (1) {
                (void)adc_sequence_init_dt(&adc_channels[0], &sequence);
                int err = adc_read(adc_channels[0].dev, &sequence);
                if (err < 0) {
                        printk("Could not read (%d)\n", err);
                        k_sleep(K_MSEC(100));
                        continue;
                }

                int32_t sound_value = (int32_t)buf;
                if (sound_value >= SENSOR_INVALID_VALUE) {
                        printk("sound_value: invalid data %" PRIu32 "\n", sound_value);
                        k_sleep(K_MSEC(100));
                        continue;
                }

                int sound_level = map(sound_value, 0, MAX_SENSORVALUE, 0, MIN_SENSORVALUE);
                printk("sound_value: %" PRIu32 " sound_level : %d\n", sound_value, sound_level);
                k_sleep(K_MSEC(100));
        }
        return 0;
}
