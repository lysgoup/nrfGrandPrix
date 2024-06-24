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
#include "./include/co2.h"
#include "./include/sound.h"

int busy;

int configuration(){

        int ret = DK_OK;
        busy = 0;

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

        ret = buzzer_init();
        if (ret != DK_OK) {
                printk("Error initializing Buzzer\n");
                return DK_ERR;
        }

        // 로터리 인코더 초기화
        ret = rotary_init(); // rotary thread 시작
        if (ret != 0) {
                printk("Error initializing Rotary\n");
                return DK_ERR;
        } 
        
        // 배터리 디스플레이 초기화
        ret = batterydisplay_init();
        if(ret != DK_OK){
                printk("Error initializing battery display\n");
                return DK_ERR;
        }

        // INIT CO2
        ret = co2_init();
        if(ret != DK_OK){
                printk("Error initializing CO2\n");
                return DK_ERR;
        }

        // INIT sound sensor
        ret = sound_init();
        if(ret != DK_OK){
                printk("Error initializing Sound Sensor\n");
                return DK_ERR;
        }

        return DK_OK;
}

int main(void)
{
        
        printk("Program Start\n");

        int ret = configuration();
        if(ret == DK_ERR){
                printk("Configuration Error");
                return DK_ERR;
        }

        while(1) {
                if(!busy){
                        led_on_status(WAIT);
                }
                k_sleep(K_MSEC(100));
        }

        printk("End of main!");
        return DK_OK;
}
