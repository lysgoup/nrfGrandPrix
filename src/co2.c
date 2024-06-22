/*
 * Copyright (c) 2022 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>
#include "./include/value.h"
#include "./include/co2.h"
#include "./include/led.h"

struct k_work co2_work;
int co2_percent;
bool co2_work_pending;
extern int busy;

int co2_init(){
  if (!device_is_ready(uart_serial)) {
		printk("UART device not found!");
		return DK_ERR;
	}

	int ret = uart_irq_callback_user_data_set(uart_serial, serial_callback, NULL);

	if(ret < 0){
		if(ret == -ENOTSUP){
			printk("Interrupt-driven UART API support not enabled\n");
		}else if(ret == -ENOSYS){
			printk("UART device does not support interrupt-driven API\n");
		}else{
			printk("Error setting UART callback: %d\n",ret);
		}
		return DK_ERR;
	}
  co2_percent = 0;
  co2_work_pending = false;
	uart_irq_rx_enable(uart_serial);
  k_work_init(&co2_work, co2_work_handler);
  return DK_OK;
}

uint8_t check_uart_fsm(uint8_t reset, uint8_t read_data) {
	if(reset)
	   uart_fsm_state = UART_FSM_IDLE;
	else 
	    switch (uart_fsm_state) {
		case UART_FSM_IDLE:
			if (read_data == 0xFF) {
				uart_fsm_state = UART_FSM_HEADER;
			}
			break;
		case UART_FSM_HEADER:
			if (read_data == 0x86) {
				uart_fsm_state = UART_FSM_DATA;
			} else {
				uart_fsm_state = UART_FSM_IDLE;
			}
			break;
		case UART_FSM_DATA:
			if (rx_buf_pos == MSG_SIZE - 2) {
				uart_fsm_state = UART_FSM_CHECKSUM;
			}
			break;
		case UART_FSM_CHECKSUM:
			if (rx_buf_pos == MSG_SIZE - 1) {
				uart_fsm_state = UART_FSM_END;
			}
			break;
		case UART_FSM_END:
			uart_fsm_state = UART_FSM_IDLE;
			break;
		default:
			uart_fsm_state = UART_FSM_IDLE;
			break;
	}
	return uart_fsm_state;
}


unsigned char getCheckSum(char *packet) {
	unsigned char i, checksum=0;
	for( i = 1; i < 8; i++) {
		checksum += packet[i];
	}
	checksum = 0xff - checksum;
	checksum += 1;
	return checksum;
}

void serial_callback(const struct device *dev, void *user_data) {
	uint8_t c, high, low;
	char checksum_ok, value_calc_flag;
	int checksum;

	if (!uart_irq_update(uart_serial)) {
		printk("irq_update Error\n");
		return;
	}

	if (!uart_irq_rx_ready(uart_serial)) {
		printk("irq_ready: No data\n");
		return;
	}

	/* read until FIFO empty */
	while (uart_fifo_read(uart_serial, &c, 1) == 1) {
		// for recovery
		if (uart_fsm_state == UART_FSM_IDLE) {
			rx_buf_pos = 0;
		}
		check_uart_fsm(0,c);

		if (rx_buf_pos >= MSG_SIZE) {
			rx_buf_pos = 0;
		}
		rx_buf[rx_buf_pos++] = c;
	}

	// calculate checksum, and compare with received checksum
	if(uart_fsm_state == UART_FSM_END){
	  checksum = getCheckSum(rx_buf);
	  checksum_ok = (checksum == rx_buf[8]);
	  if (checksum_ok) {
		  // printk("Checksum OK (%d == %d, index=%d)\n", checksum, rx_buf[8], rx_buf_pos);
	 
	     // check if we received all data and checksum is OK
	    value_calc_flag = (rx_buf_pos == MSG_SIZE);
	    if (value_calc_flag) {
        high = rx_buf[2];
        low = rx_buf[3];
        int ppm = (high * CO2_MULTIPLIER) + low;
        printk("CO2: %d ppm (high = %d, low = %d)\n", ppm , high, low);
        co2_percent = (float)ppm / CO2_MAX_VALUE * 100;
        printk("check: %d\n",co2_percent);
        // print message buffer
        for (int i = 0; i < MSG_SIZE; i+=1) {
          printk("%x ", rx_buf[i]);
        }
        printk("\n");
		  }
	  } 
	  else {
		printk("Checksum failed (%d == %d, index=%d)\n", checksum, rx_buf[8], rx_buf_pos);
	  }
	  check_uart_fsm(1,0); // reset
	}
  if (!co2_work_pending) {
    co2_work_pending = true;
    k_work_submit(&co2_work);
  }
}

void serial_write() {
	uint8_t tx_buf[MSG_SIZE] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
	for (int i = 0; i < MSG_SIZE; i+=1) {
		uart_poll_out(uart_serial, tx_buf[i]);
	}
}

void co2_work_handler(struct k_work *work){
  printk("Enter co2_work_handler: %d\n",co2_percent);
  busy = 1;
  led_on_co2_value(co2_percent);
  k_sleep(K_SECONDS(3));
  co2_work_pending = false;
  busy=0;
  printk("Leaving co2_work_handler\n");
}