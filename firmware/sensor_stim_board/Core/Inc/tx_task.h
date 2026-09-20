#ifndef _TX_TASK_H_
#define _TX_TASK_H_

void tx_task(void);

void tx_cmd_task(void);

void uart1_it_handler(void);

void cmd_parse(uint8_t *buf, uint16_t len);

#endif
