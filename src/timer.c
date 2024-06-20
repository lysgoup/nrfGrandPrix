#include "./include/timer.h"
#include "./include/led.h"
#include "./include/joy.h"
#include "./include/buzzer.h"
struct k_timer my_timer;
struct k_work my_work;

int seconds = 0;
int move = STAY;
bool timer_stopped = true;
double timer_period = 0.1;
int game_duration = 5; // the time for lasting game - current: 30s

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

    seconds++; 
    //if (seconds > game_duration * (1/timer_period)) { // 30초가 지나면 종료. 1/timer_period -> 0.2s에 한 번씩 seconds 증가하므로
    if(ret == 1){
        k_timer_stop(&my_timer);
        seconds = 0;
        led_on_status(PASS);
        led_blink_status(PASS, BLINK_ON_TIME, BLINK_OFF_TIME);
        timer_stopped = true;
        stop_joystick_thread();
        printk("Game Done\n");  
    }
    if(ret == -1){
        k_timer_stop(&my_timer);
        seconds = 0;
        led_on_status(FAIL);
        led_blink_status(FAIL, BLINK_ON_TIME, BLINK_OFF_TIME);
        timer_stopped = true;
        stop_joystick_thread();
        printk("Game Done\n");  
    }
}

K_WORK_DEFINE(my_work, my_work_handler);

void my_timer_handler(struct k_timer *dummy)
{
    k_work_submit(&my_work);
}

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);
