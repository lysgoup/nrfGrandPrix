#define ADC_BUFFER_SIZE 1 // 단일 값 읽기를 위한 버퍼 크기
#define SENSOR_INVALID_VALUE 65535 // ADC 값의 유효 범위를 벗어나는 값으로 설정
#define MAX_SENSORVALUE 1023 // 10비트 ADC의 최대 값
#define MIN_SENSORVALUE 0 // ADC의 최소 값

static int16_t buf[ADC_BUFFER_SIZE]; // ADC 값을 저장할 버퍼

struct adc_sequence sequence = {
    .buffer = buf,
    .buffer_size = sizeof(buf),
    .resolution = 10, // 10비트 해상도 설정
};

static const struct adc_dt_spec adc_channels[] = {
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
};