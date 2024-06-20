#include "./include/rotary.h"
#include "./include/led.h"

K_THREAD_STACK_DEFINE(rotary_stack, STACK_SIZE);
struct k_thread rotary_thread_data;
static bool stop_thread = false;
static int brightness = INIT_BRIGHTNESS;

static void rotary_thread(void *arg1, void *arg2, void *arg3)
{
    const struct device *const dev = DEVICE_DT_GET(DT_ALIAS(qdec0));
    struct sensor_value val;
    int rc;

    while (!stop_thread) {
        rc = sensor_sample_fetch(dev);
        if (rc != 0) {
            printk("Failed to fetch sample (%d)\n", rc);
            continue;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ROTATION, &val);
        if (rc != 0) {
            printk("Failed to get data (%d)\n", rc);
            continue;
        }

        if (val.val1 > 0) {
            brightness += 10;
        } else if (val.val1 < 0) {
            brightness -= 10;
        }

        if (brightness < 0) {
            brightness = 0;
        } else if (brightness > 100) {
            brightness = 100;
        }

        led_set_brightness(led, 0, brightness);

        printk("Brightness = %d\n", brightness);
        k_msleep(100);
    }
    printk("Rotary thread stopped.\n");
}

void start_rotary_thread(void)
{
    stop_thread = false;
    k_thread_create(&rotary_thread_data, rotary_stack,
                    K_THREAD_STACK_SIZEOF(rotary_stack),
                    rotary_thread,
                    NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
}

void stop_rotary_thread(void)
{
    stop_thread = true;
    k_thread_join(&rotary_thread_data, K_FOREVER);
}

int rotary_init(void)
{
    const struct device *const dev = DEVICE_DT_GET(DT_ALIAS(qdec0));
    if (!device_is_ready(dev)) {
        printk("Qdec device is not ready\n");
        return -1;
    }

    start_rotary_thread();
    return 0;
}
