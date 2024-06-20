#include "./include/led.h"
#include "./include/value.h"

int number_led_matrix_arr [MAX_LED_MATRIX_IDX+1][MAX_LED_MATRIX_NUM+1];
int led_matrix_arr [3][MAX_LED_NUM+1];
int led_map [MAX_LED_MATRIX_ROW][MAX_LED_MATRIX_COL*6];

int carPose = MID;

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

    led_set_brightness(led, 0, INIT_BRIGHTNESS);

    return DK_OK;
}

// LEDs of Car

void led_mid(int on){
    for(int i = 49; i < 68 ; i+=16){
        for(int j = i ; j < i+3 ; j++){
            if(j==66) continue;
            if(on) led_on(led, j);
            else led_off(led, j);
        }
    }
}

void led_top(int on){
    for(int i = 1; i < 19 ; i+=16){
        for(int j = i ; j < i+3 ; j++){
            if(j==18) continue;
            if(on) led_on(led, j);
            else led_off(led, j);
        }
    }
}

void led_bot(int on){
    for(int i = 97; i < 115 ; i+=16){
        for(int j = i ; j < i+3 ; j++){
            if(j==114) continue;
            if(on) led_on(led, j);
            else led_off(led, j);
        }
    }
}

void led_on_top(){
    led_top(ON);
    led_mid(OFF);
    led_bot(OFF);
}

void led_on_mid(){
    led_top(OFF);
    led_mid(ON);
    led_bot(OFF);
}

void led_on_bot(){
    led_top(OFF);
    led_mid(OFF);
    led_bot(ON);
}


// LED TEXT DISPLAY

void led_on_seconds(int num)
{
    int tens = num / 10;
    int units = num % 10;

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

void led_on_status(int type){
    for(int i = 0; i < MAX_LED_NUM; i++){
        if(led_matrix_arr[type][i] == 1){
            // printk("[tens] led_on: j:[%d] num_array_idx[%d]\n", j, num_arr_idx);
            led_on(led, i);
        } else {
            led_off(led, i);
        }
    }
}

void led_blink_status(int type, uint32_t on_time, uint32_t off_time){
    for(int i = 0 ; i <=MAX_LED_NUM; i++){
        if(led_matrix_arr[type][i]){
            led_blink(led, i, on_time, off_time);
        }
    }
    k_msleep(3000);
    led_blink(led, 0, 0, 0);
}

int check_collision(int carX, int carY) { // top-left point of the car
    for (int i = 0; i <= 2; i++) {
        if (led_map[carX][carY + i] == 1 || led_map[carX+1][carY + i] == 1) {
            return -1;
        }
    }
    return 0;
}


// FOR GAME

int show_map(int second, int move){

    printk("carPose: %d\n", carPose);

    if(second == 0) carPose = MID; // Pose Init

    // MAP

    int start_col = second % (MAX_LED_MATRIX_COL * MAP_LENGTH + 1 - MAX_LED_MATRIX_COL); // 시작 열 계산
    if(start_col == 0 && second != 0){return 1;} // 마지막 순간

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 16; col++) {
            int led_index = row * 16 + col;
            if (led_map[row][start_col + col]) {
                led_on(led, led_index); // 해당 LED를 켜기
            } else {
                led_off(led, led_index); // 해당 LED를 끄기
            }
        }
    }

    // CAR

    // Car Movement Check

    if(move == UP){
        if(carPose != TOP){ // 이미 TOP에 있는 경우는 제외
            carPose--;
        }
    }
    else if(move == DOWN){
        printk("moveDown!\n");
        if(carPose != BOT){ // 이미 BOT에 있는 경우는 제외
            carPose++;
        }
    }

    // Car Draw

    if(carPose == MID){
        led_on_mid();
        return check_collision(3, start_col+1);
    }
    else if(carPose == TOP){
        led_on_top();
        return check_collision(0, start_col+1);
    }
    else{
        led_on_bot();
        return check_collision(6, start_col+1);
    }

    return 0;

}

int led_matrix_arr [3][MAX_LED_NUM+1]= {
    {//WAIT
        0,0,0,0,0, 0,0,0,0, 0, 0,0,0, 0,0,0,
        0,0,0,0,0, 0,0,0,0, 0, 0,0,0, 0,0,0,
        1,0,1,0,1, 0,1,1,0, 0, 1,1,1, 1,1,1,
        1,0,1,0,1, 1,0,0,1, 0, 0,1,0, 0,1,0,
        1,0,1,0,1, 1,1,1,1, 0, 0,1,0, 0,1,0,
        1,0,1,0,1, 1,0,0,1, 0, 0,1,0, 0,1,0,
        0,1,0,1,0, 1,0,0,1, 0, 1,1,1, 0,1,0,
        0,0,0,0,0, 0,0,0,0, 0, 0,0,0, 0,0,0
    },
    {//PASS
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1,
        1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,
        1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,1,
        1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    },
    {//FAIL
        0,0,0,0,0,0,0,0, 0,0,0,0, 0,0,0,0,
        0,0,0,0,0,0,0,0, 0,0,0,0, 0,0,0,0,
        1,1,1,1,0,1,0,0, 1,1,1,0, 1,0,0,0,
        1,0,0,0,1,0,1,0, 0,1,0,0, 1,0,0,0,
        1,1,1,0,1,1,1,0, 0,1,0,0, 1,0,0,0,
        1,0,0,1,0,0,0,1, 0,1,0,0, 1,0,0,0,
        1,0,0,1,0,0,0,1, 1,1,1,0, 1,1,1,1,
        0,0,0,0,0,0,0,0, 0,0,0,0, 0,0,0,0
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

int led_map [8][MAX_LED_MATRIX_COL*MAP_LENGTH] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},  
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1}, 
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},  
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},  
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1}
};