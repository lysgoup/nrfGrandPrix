#ifndef TIMER_H
#define TIMER_H

#include <zephyr/kernel.h>

// 타이머 관련 변수 선언
extern struct k_timer my_timer;
extern struct k_work my_work;
extern int seconds;
extern bool timer_stopped;
extern double timer_period;
extern int secondMaker;
extern int game_duration;

// 타이머 핸들러 및 작업 함수 선언
void my_work_handler(struct k_work *work);
void my_timer_handler(struct k_timer *dummy);

#endif // TIMER_H
