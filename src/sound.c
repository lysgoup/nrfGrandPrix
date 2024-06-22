#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <inttypes.h>

#include "./include/sound.h"
#include "./include/value.h"

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int sound_init(){
  for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
    if (!adc_is_ready_dt(&adc_channels[i])) {
      printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
      return DK_ERR;
    }

    int err = adc_channel_setup_dt(&adc_channels[i]);
    if (err < 0) {
      printk("Could not setup channel #%d (%d)\n", i, err);
      return DK_ERR;
    }
  }
  return DK_OK;
}

// int main(void)
// {
//     while (1) {
//         (void)adc_sequence_init_dt(&adc_channels[0], &sequence);
//         int err = adc_read(adc_channels[0].dev, &sequence);
//         if (err < 0) {
//             printk("Could not read (%d)\n", err);
//             k_sleep(K_MSEC(100));
//             continue;
//         }

//         // 버퍼의 첫 번째 값을 읽어 sound_value로 저장
//         int32_t sound_value = buf[0]; 
//         // sound_value가 부호 없는 정수로 잘못 해석되는 경우를 방지
//         if (sound_value < 0 || sound_value >= SENSOR_INVALID_VALUE) {
//             printk("sound_value: invalid data %" PRIi32 "\n", sound_value);
//             k_sleep(K_MSEC(100));
//             continue;
//         }

//         int sound_level = map(sound_value, MIN_SENSORVALUE, MAX_SENSORVALUE, 0, 100); // 0-100 범위로 매핑
//         printk("sound_value: %" PRIi32 " sound_level: %d\n", sound_value, sound_level);
//         k_sleep(K_MSEC(100));
//     }
//     return 0;
// }
