#ifndef _RX_TASK_H_
#define _RX_TASK_H_

void start_uart3_rx(void);

void uart3_it_handler(void);

void start_uart2_rx(void);

void rx_task(void);

extern uint16_t stim_matrix[];
extern volatile uint8_t stim_matrix_active;
extern volatile uint32_t stim_matrix_last_rx_ms;

#endif
