#ifndef _TAZER_H_
#define _TAZER_H_

void tazer_test_task(void);

void tazer_current_task(void);

void tazer_fast_task(void);

void tazer_25hz_task(void);

void tazer_stop_task(void);

void tazer_matrix_scan_task(void);

typedef enum
{
    TAZER_STATE_OFF,
    TAZER_STATE_ON = 100,
} tazer_state_t;

extern uint32_t tz_start_cnt;
extern uint8_t if_tazing;

#endif
