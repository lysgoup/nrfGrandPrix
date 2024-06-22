#include "./include/timer.h"
#include "./include/led.h"
#include "./include/joy.h"
#include "./include/buzzer.h"
#include "./include/batteryDisplay.h"
#include <math.h>

struct k_timer my_timer;
struct k_work my_work;

int seconds = 0;
int move = STAY;
bool timer_stopped = true;
double timer_period = 0.1;


void isEnd(int status){

    if(status==0) return;

    k_timer_stop(&my_timer);
    seconds = 0;
    led_on_status(status);
    led_blink_status(status, BLINK_ON_TIME, BLINK_OFF_TIME);
    timer_stopped = true;
    stop_joystick_thread();
    display_clear();
    printk("Game Done\n");  

}

void my_work_handler(struct k_work *work)
{
    if(seconds==0){
        for(int i=3;i>0;i--){
            led_on_seconds(i);
            buzzer_on(659,500);
            buzzer_off(500);
            // k_sleep(K_MSEC(1000));
        }
        start_joystick_thread();
    }
    
    if(seconds==1){
        buzzer_on(1319,300);
        buzzer_off(0);
    }
    
    move = joyCheckMove();
    int ret = show_map(seconds, move);

    int interval = round(getMapLength() / 7); // 7 = batteryDisplay levels

    if(seconds==0) display_level(0);
    else display_level((seconds/interval)+1); // 1 ~ intveral-1 -> level 1, interval ~ inverval*2 - 1 -> level 2

    isEnd(ret);

    seconds++; 
}

K_WORK_DEFINE(my_work, my_work_handler);

void my_timer_handler(struct k_timer *dummy)
{
    k_work_submit(&my_work);
}

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);
