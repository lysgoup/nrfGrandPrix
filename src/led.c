#include "./led.h"
#include "./value.h"

int number_led_matrix_arr [MAX_LED_MATRIX_IDX+1][MAX_LED_MATRIX_NUM+1];
int fail_led_matrix_arr [2][MAX_LED_MATRIX_NUM+1];
int pass_led_matrix_arr [2][MAX_LED_MATRIX_NUM+1];

// Function to initialize the LED matrix
int led_init(void)
{
    if(!device_is_ready(led)){
        printk("LED device %s is not ready\n", led->name);
        return DK_ERR;
    }

    for(int i = 0; i< MAX_LED_NUM; i++){
        led_off(led, i);
    }

    return DK_OK;
}

void led_on_seconds(int seconds)
{
    int tens = seconds / 10;
    int units = seconds % 10;

    int num_arr_idx = 0;

    for(int i = 0; i < MAX_LED_NUM; i+=16){
        for(int j = i; j < (i+8); j++){
            if(number_led_matrix_arr[tens][num_arr_idx] == 1){
                // printk("[tens] led_on: j:[%d] num_array_idx[%d]\n", j, num_arr_idx);
                led_on(led, j);
            } else {
                led_off(led, j);
            }

            num_arr_idx++;
        }
    }

    num_arr_idx = 0;

    for(int i = 0; i < MAX_LED_NUM; i+=16){
        for(int j = (i+8); j < (i+16); j++){
            if(number_led_matrix_arr[units][num_arr_idx] == 1){
                // printk("[units] led_on: j:[%d] num_array_idx[%d]\n", j, num_arr_idx);
                led_on(led, j);
            } else {
                led_off(led, j);
            }
            num_arr_idx++;
        }
    }
}

void led_on_result(){
    int num_arr_idx = 0;

    for(int i = 0; i < MAX_LED_NUM; i+=16){
        for(int j = i; j < (i+8); j++){
            if(pass_led_matrix_arr[0][num_arr_idx] == 1){
                // printk("[tens] led_on: j:[%d] num_array_idx[%d]\n", j, num_arr_idx);
                led_on(led, j);
            } else {
                led_off(led, j);
            }

            num_arr_idx++;
        }
    }

    num_arr_idx = 0;

    for(int i = 0; i < MAX_LED_NUM; i+=16){
        for(int j = (i+8); j < (i+16); j++){
            if(pass_led_matrix_arr[1][num_arr_idx] == 1){
                // printk("[units] led_on: j:[%d] num_array_idx[%d]\n", j, num_arr_idx);
                led_on(led, j);
            } else {
                led_off(led, j);
            }
            num_arr_idx++;
        }
    }
}

int pass_led_matrix_arr [2][MAX_LED_MATRIX_NUM+1]= {
    {//FA
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        1,1,1,0,0,1,1,0,
        1,0,0,1,1,0,0,1,
        1,1,1,0,1,1,1,1,
        1,0,0,0,1,0,0,1,
        1,0,0,0,1,0,0,1,
        0,0,0,0,0,0,0,0
    },
    {//IL
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,
        1,0,0,0,1,0,0,0,
        1,1,1,1,1,1,1,1,
        0,0,0,1,0,0,0,1,
        1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0
    }
};

int fail_led_matrix_arr [2][MAX_LED_MATRIX_NUM+1]= {
    {//FA
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        1,1,1,1,0,1,0,0,
        1,0,0,0,1,0,1,0,
        1,1,1,0,1,1,1,0,
        1,0,0,1,0,0,0,1,
        1,0,0,1,0,0,0,1,
        0,0,0,0,0,0,0,0
    },
    {//IL
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        1,1,1,0,1,0,0,0,
        0,1,0,0,1,0,0,0,
        0,1,0,0,1,0,0,0,
        0,1,0,0,1,0,0,0,
        1,1,1,0,1,1,1,1,
        0,0,0,0,0,0,0,0
    }
};

int number_led_matrix_arr [MAX_LED_MATRIX_IDX+1][MAX_LED_MATRIX_NUM+1]= {
    { // 0
        0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,0
    },
    { // 1
        0,0,0,0,1,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,1,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,1,1,1,1,1,0
    },
    { // 2
        0,0,0,0,1,0,0,0,
        0,0,0,1,0,1,0,0,
        0,0,1,0,0,0,1,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,1,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,1,0
    },
    { // 3
        0,0,1,1,1,1,0,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,1,1,1,1,0,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,1,1,1,1,0,0
    },
    { // 4
        0,0,0,0,0,1,0,0,
        0,0,0,0,1,1,0,0,
        0,0,0,1,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,1,1,1,1,1,1,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,0,1,0,0
    },
    { // 5
        0,0,1,1,1,1,1,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,0
    },
    { // 6
        0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,0
    },
    { // 7
        0,0,1,1,1,1,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,1,0
    },
    { // 8
        0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,0
    },
    { // 9
        0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,1,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,1,0
    }
};