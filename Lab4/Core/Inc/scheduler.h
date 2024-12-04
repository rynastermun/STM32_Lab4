/*
 * scheduler.h
 *
 *  Created on: Dec 3, 2024
 *      Author: ADMIN PC
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"

#define SCH_MAX_TASKS	40
#define	NO_TASK_ID 		0

typedef struct {
    void (*pTask)(void); 		// Con trỏ hàm: trỏ đến hàm thực thi task
    uint32_t Delay;             // Thời gian trễ trước khi thực thi
    uint32_t Period;            // Chu kỳ lặp lại của task
    uint8_t RunMe;         		// Cờ báo hiệu task sẵn sàng thực thi
    uint32_t taskID;
} sTask;

void SCH_Init(void);
void SCH_Update(void);
unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int Delay, unsigned int Period);
void SCH_Delete_Task(const int task_index);
void SCH_Dispatch_Tasks(void);

#endif /* INC_SCHEDULER_H_ */
