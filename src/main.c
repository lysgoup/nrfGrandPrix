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
#include "./include/batteryDisplay.h"

int configuration(){

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

        // buzzer 초기화
        init_buzzer();

        // 로터리 인코더 초기화
        ret = rotary_init(); // rotary thread 시작
        if (ret != 0) {
                printk("Error initializing Rotary\n");
                return DK_ERR;
        } 
        
        // 배터리 디스플레이 초기화
        ret = batterydisplay_intit();
        if(ret != DK_OK){
                printk("Error initializing battery display\n");
                return DK_ERR;
        }

        return DK_OK;
}

int main(void)
{
        
        printk("Program Start");

        int ret = configuration();
        if(ret == DK_ERR){
                printk("Configuration Error");
                return DK_ERR;
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
