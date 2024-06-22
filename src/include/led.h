#include <zephyr/drivers/led.h>
#include <zephyr/drivers/kscan.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>

#define INIT_BRIGHTNESS 30

#define MAX_LED_MATRIX_IDX 10
#define MAX_LED_NUM 128
#define MAX_LED_MATRIX_NUM 64

#define MAX_LED_MATRIX_COL 16
#define MAX_LED_MATRIX_ROW 8
#define MAP_INTERVAL 13

#define TENS_START_INDEX 0
#define UNITS_START_INDEX 8

#define LED_NODE DT_COMPAT_GET_ANY_STATUS_OKAY(holtek_ht16k33)
#define KEY_NODE DT_CHILD(LED_NODE, keyscan)

#define WAIT 0
#define PASS 1
#define FAIL 2

#define TOP 0
#define MID 1
#define BOT 2

#define UP 0
#define STAY 1
#define DOWN 2

#define ON 1
#define OFF 0

#define BLINK_ON_TIME 500
#define BLINK_OFF_TIME 500

static const struct device *const led = DEVICE_DT_GET(LED_NODE);

int led_init(void);
void led_on_seconds(int seconds);
void led_on_co2_value(int value);
void led_on_status(int type);
void led_blink_status(int type, uint32_t on_time, uint32_t off_time);
int show_map(int second, int pos);
int getMapLength();
