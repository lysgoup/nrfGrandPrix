#include <zephyr/sys/util.h>

#include "./include/gpios.h"
#include "./include/timer.h"
#include "./include/led.h"
#include "./include/batteryDisplay.h"
#include "./include/sound.h"

static struct gpio_callback button0_cb_data; // Game 진입
static struct gpio_callback button1_cb_data;
static struct gpio_callback button2_cb_data;
static struct gpio_callback button3_cb_data;

extern int busy;

void button0_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button 0 pressed\n");
    if(busy == 1){
        return;
    }
    busy = 1;
    if (timer_stopped) {
        timer_stopped = false;  // 타이머가 시작됨을 표시
        seconds = 0;  // 타이머 초기화 
        //display_level(4);
        k_timer_start(&my_timer, K_SECONDS(timer_period), K_SECONDS(timer_period));  // 타이머 시작
        printk("Timer started\n");
    }
}

void button1_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button 1 pressed\n");
    if(busy==1){
        return;
    }
    serial_write();
}

void button2_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button 2 pressed\n");
    if(sound_mode_on && busy){
        printk("turning off sound mode\n"); 
        turn_off_sound_mode();
        printk("sound mode on: %d, busy: %d\n",sound_mode_on,busy);
    }
    else if(!sound_mode_on && !busy){
        printk("turning on sound mode\n"); 
        turn_on_sound_mode();
        printk("sound mode on: %d, busy: %d\n",sound_mode_on,busy);
    }
}

void button3_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button 3 pressed\n");
    int ret;
    ret = gpio_pin_toggle_dt(&led3);
    if (ret < 0) {
            return;
    }
}

int gpio_init(void)
{
    int err = GPIO_FAIL;

    // Set butto0 interrupt
    printk("Setting button0 interrupt\n");

    err = gpio_is_ready_dt(&button0);
    if (!err) {
        printk("Error gpio_is_ready_dt led0 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_configure_dt(&button0, GPIO_INPUT | GPIO_PULL_UP);
    if (err < 0) {
        printk("Error configuring button0 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_interrupt_configure_dt(&button0, GPIO_INT_EDGE_TO_ACTIVE);
    if (err != 0) {
        printk("Error configuring interrupt on button0 pin %d\n", err);
        return GPIO_FAIL;
    }
    gpio_init_callback(&button0_cb_data, button0_callback, BIT(button0.pin));
    gpio_add_callback(button0.port, &button0_cb_data);


    // Set button1 interrupt
    printk("Setting button1 interrupt\n");
    err = gpio_is_ready_dt(&button1);
    if (!err) {
        printk("Error gpio_is_ready_dt led1 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_configure_dt(&button1, GPIO_INPUT | GPIO_PULL_UP);
    if (err < 0) {
        printk("Error configuring button1 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
    if (err != 0) {
        printk("Error configuring interrupt on button1 pin %d\n", err);
        return GPIO_FAIL;
    }
    gpio_init_callback(&button1_cb_data, button1_callback, BIT(button1.pin));
    gpio_add_callback(button1.port, &button1_cb_data);

    // Set button2 interrupt
    printk("Setting button2 interrupt\n");
    err = gpio_is_ready_dt(&button2);
    if (!err) {
        printk("Error gpio_is_ready_dt led2 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_configure_dt(&button2, GPIO_INPUT | GPIO_PULL_UP);
    if (err < 0) {
        printk("Error configuring button2 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_interrupt_configure_dt(&button2, GPIO_INT_EDGE_TO_ACTIVE);
    if (err != 0) {
        printk("Error configuring interrupt on button2 pin %d\n", err);
        return GPIO_FAIL;
    }
    gpio_init_callback(&button2_cb_data, button2_callback, BIT(button2.pin));
    gpio_add_callback(button2.port, &button2_cb_data);

    // Set button3 interrupt
    printk("Setting button3 interrupt\n");
    err = gpio_is_ready_dt(&button3);
    if (!err) {
        printk("Error gpio_is_ready_dt led3 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_configure_dt(&button3, GPIO_INPUT | GPIO_PULL_UP);
    if (err < 0) {
        printk("Error configuring button3 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_interrupt_configure_dt(&button3, GPIO_INT_EDGE_TO_ACTIVE);
    if (err != 0) {
        printk("Error configuring interrupt on button3 pin %d\n", err);
        return GPIO_FAIL;
    }
    gpio_init_callback(&button3_cb_data, button3_callback, BIT(button3.pin));
    gpio_add_callback(button3.port, &button3_cb_data);
    
    /* LED setting (nrf52840) - unuse
    // Set led0
    err = gpio_is_ready_dt(&led0);
    if (!err) {
        printk("Error gpio_is_ready_dt led0 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    if (err < 0) {
        printk("Error %d: failed to configure LED0 pin\n", err);
        return GPIO_FAIL;
    }

    // Set led1
    err = gpio_is_ready_dt(&led1);
    if (!err) {
        printk("Error gpio_is_ready_dt led1 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if (err < 0) {
        printk("Error %d: failed to configure LED1 pin\n", err);
        return GPIO_FAIL;
    }

    // Set led2
    err = gpio_is_ready_dt(&led2);
    if (!err) {
        printk("Error gpio_is_ready_dt led2 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
    if (err < 0) {
        printk("Error %d: failed to configure LED2 pin\n", err);
        return GPIO_FAIL;
    }

    // Set led3
    err = gpio_is_ready_dt(&led3);
    if (!err) {
        printk("Error gpio_is_ready_dt led3 pin %d\n", err);
        return GPIO_FAIL;
    }

    err = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);
    if (err < 0) {
        printk("Error %d: failed to configure LED3 pin\n", err);
        return GPIO_FAIL;
    }

    */

    return GPIO_OK;
}