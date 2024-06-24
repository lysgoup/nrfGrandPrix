#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <inttypes.h>

#include "./include/sound.h"
#include "./include/value.h"
#include "./include/led.h"

struct k_work sound_work;
bool sound_work_pending;
int sound_percent;
bool sound_mode_on;
extern int busy;

extern int map(int x, int in_min, int in_max, int out_min, int out_max);

struct adc_sequence sequence = {
    .buffer = buf,
    .buffer_size = sizeof(buf),
    .resolution = 10, // 10비트 해상도 설정
};

// int map(int x, int in_min, int in_max, int out_min, int out_max) {
//     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }

int sound_init(){
  for (size_t i = 0U; i < ARRAY_SIZE(sound_adc_channels); i++) {
    if (!adc_is_ready_dt(&sound_adc_channels[i])) {
      printk("ADC controller device %s not ready\n", sound_adc_channels[i].dev->name);
      return DK_ERR;
    }

    int err = adc_channel_setup_dt(&sound_adc_channels[i]);
    if (err < 0) {
      printk("Could not setup channel #%d (%d)\n", i, err);
      return DK_ERR;
    }
  }
  sound_work_pending = false;
  sound_mode_on = false;
  sound_percent = 0;
  k_work_init(&sound_work, sound_work_handler);
  return DK_OK;
}

void turn_on_sound_mode(){
  if (!sound_work_pending) {
    sound_work_pending = true;
    k_work_submit(&sound_work);
  }
}

void turn_off_sound_mode(){
  sound_mode_on = false;

}

void sound_work_handler(struct k_work *work){
  printk("Enter sound_work_handler: %d\n",sound_percent);
  sound_mode_on = true;
  busy = 1;
  while(sound_mode_on){
    printk("check: %d\n",sound_mode_on);
    (void)adc_sequence_init_dt(&sound_adc_channels[2], &sequence);
    int err = adc_read(sound_adc_channels[2].dev, &sequence);
    if (err < 0) {
        printk("Could not read (%d)\n", err);
        k_sleep(K_MSEC(100));
        continue;
    }

    // 버퍼의 첫 번째 값을 읽어 sound_value로 저장
    int32_t sound_value = buf[0]; 
    // sound_value가 부호 없는 정수로 잘못 해석되는 경우를 방지
    if (sound_value < 0 || sound_value >= SENSOR_INVALID_VALUE) {
        printk("sound_value: invalid data %" PRIi32 "\n", sound_value);
        k_sleep(K_MSEC(100));
        continue;
    }
    printk("detected: %d\n",sound_value);
    int sound_level = map(sound_value, MIN_SENSORVALUE, MAX_SENSORVALUE, 0, 100); // 0-100 범위로 매핑
    printk("sound_value: %" PRIi32 " sound_level: %d\n", sound_value, sound_level);
    led_on_percentage(sound_level);
    k_sleep(K_MSEC(500));
  }
  sound_work_pending = false;
  busy=0;
  printk("Leaving sound_work_handler\n");
}
