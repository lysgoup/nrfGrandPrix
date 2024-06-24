#ifndef SOUND_HEADER
#define SOUND_HEADER

#include <zephyr/drivers/adc.h>

#define ADC_BUFFER_SIZE 1 // 단일 값 읽기를 위한 버퍼 크기
#define SENSOR_INVALID_VALUE 65535 // ADC 값의 유효 범위를 벗어나는 값으로 설정
#define MAX_SENSORVALUE 1023 // 10비트 ADC의 최대 값
#define MIN_SENSORVALUE 0 // ADC의 최소 값

static int16_t buf[ADC_BUFFER_SIZE]; // ADC 값을 저장할 버퍼
extern bool sound_mode_on;


static const struct adc_dt_spec sound_adc_channels[] = {
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 2)
};

int sound_init();
// int map(int x, int in_min, int in_max, int out_min, int out_max);
void turn_on_sound_mode();
void sound_work_handler(struct k_work *work);

#endif