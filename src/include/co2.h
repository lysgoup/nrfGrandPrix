#ifndef CO2_HEADER
#define CO2_HEADER

#define RECEIVE_TIMEOUT 1000

#define MSG_SIZE 9
#define CO2_MULTIPLIER 256

//K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

static const struct device *const uart_serial = DEVICE_DT_GET(DT_N_ALIAS_myserial);

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos=0;

enum uart_fsm_state_code {
	UART_FSM_IDLE,
	UART_FSM_HEADER,
	UART_FSM_DATA,
	UART_FSM_CHECKSUM,
	UART_FSM_END,
};

static uint8_t uart_fsm_state = UART_FSM_IDLE; // reset

int co2_init();
uint8_t check_uart_fsm(uint8_t reset, uint8_t read_data);
unsigned char getCheckSum(char *packet);
void serial_callback(const struct device *dev, void *user_data);
void serial_write();

#endif