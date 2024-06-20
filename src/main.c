#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/util.h>

#include "./include/led.h"
#include "./include/value.h"
#include "./include/gpios.h"
#include "./include/timer.h"
#include "./include/joy.h"
#include "./include/buzzer.h"
#include "./include/rotary.h"


int main(void)
{
        init_buzzer();
        printk("Program Start");

        int ret = DK_OK;

        // LED and buttons configuration (NRF52840's)
        ret = gpio_init();
        if (ret != DK_OK) {
            return DK_ERR;
        }

        // LED Matrix configuration
        ret = led_init();
        if(ret != DK_OK){
                printk("Error initializing LED\n");
                return DK_ERR;
        }

        // Init joystick
        ret = joystick_init();
        if (ret != DK_OK) {
                printk("Error initializing Joystick\n");
                return DK_ERR;
        }

        // 로터리 인코더 초기화
        ret = rotary_init();
        if (ret != 0) {
                printk("Error initializing Rotary\n");
                return;
        } 
        led_on_status(WAIT);

        while(1) {
                if(timer_stopped){
                        led_on_status(WAIT);
                }
                k_sleep(K_MSEC(100));
        }

        printk("End of main!");
        return DK_OK;
}
