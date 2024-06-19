#include "./include/timer.h"
#include "./include/led.h"

struct k_timer my_timer;
struct k_work my_work;

int seconds = 0;
bool timer_stopped = true;
double timer_period = 0.2;
int game_duration = 5; // the time for lasting game - current: 30s

void my_work_handler(struct k_work *work)
{
    printk("Time: %d\n", seconds);

    led_on_seconds(seconds);

    seconds++; 
    if (seconds > game_duration * (1/timer_period)) { // 30초가 지나면 종료. 1/timer_period -> 0.2s에 한 번씩 seconds 증가하므로
        seconds = 0;
        led_on_status(PASS);
        led_blink_status(PASS, BLINK_ON_TIME, BLINK_OFF_TIME);
        k_timer_stop(&my_timer);
        timer_stopped = true;
        printk("Game Done\n");  
    }
}

K_WORK_DEFINE(my_work, my_work_handler);

void my_timer_handler(struct k_timer *dummy)
{
    k_work_submit(&my_work);
}

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);
