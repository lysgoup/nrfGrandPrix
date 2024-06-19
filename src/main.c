#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/util.h>

#include "./include/led.h"
#include "./include/value.h"
#include "./include/gpios.h"
#include "./include/timer.h"


int main(void)
{
        
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

        led_on_status(WAIT);

        while(1) {
                if(timer_stopped){
                        led_on_status(WAIT);
                }
                k_sleep(K_MSEC(100));
        }

        // Start the timer
        k_timer_start(&my_timer, K_SECONDS(0.2), K_SECONDS(0.2));
        printk("End of main!");
        return DK_OK;
}
