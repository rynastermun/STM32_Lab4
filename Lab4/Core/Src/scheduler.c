/*
 * scheduler.c
 *
 *  Created on: Dec 2, 2024
 *      Author: ADMIN PC
 */

#include "scheduler.h"


sTask SCH_tasks_G[SCH_MAX_TASKS];

void SCH_Init(void) {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = NULL;
        SCH_tasks_G[i].Delay = 0;
        SCH_tasks_G[i].Period = 0;
        SCH_tasks_G[i].RunMe = 0;
    }
}

// Hàm cập nhật trạng thái nhiệm vụ
void SCH_Update(void) {
    for (uint8_t idx = 0; idx < SCH_MAX_TASKS; idx++) {
        if (SCH_tasks_G[idx].pTask) { // Nếu nhiệm vụ tồn tại
            if (SCH_tasks_G[idx].Delay == 0) {
                SCH_tasks_G[idx].RunMe++; // Đánh dấu nhiệm vụ đã sẵn sàng
                if (SCH_tasks_G[idx].Period > 0) {
                    SCH_tasks_G[idx].Delay = SCH_tasks_G[idx].Period; // Đặt lại thời gian trễ
                }
            } else {
                SCH_tasks_G[idx].Delay--; // Giảm thời gian trễ
            }
        }
    }
}

unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int Delay, unsigned int Period) {
    unsigned char idx = 0;

    // Vòng lặp tìm kiếm vị trí trống trong mảng nhiệm vụ
    for (idx = 0; idx < SCH_MAX_TASKS; idx++) {
        if (SCH_tasks_G[idx].pTask == NULL) {
            break; // Dừng lại nếu tìm thấy vị trí trống
        }
    }

    // Kiểm tra nếu danh sách đầy
    if (idx == SCH_MAX_TASKS) {
        return SCH_MAX_TASKS; // Trả về lỗi nếu không còn vị trí
    }

    // Thêm nhiệm vụ mới vào vị trí trống
    SCH_tasks_G[idx].pTask = pFunction;
    SCH_tasks_G[idx].Delay = Delay;
    SCH_tasks_G[idx].Period = Period;
    SCH_tasks_G[idx].RunMe = 0;

    return idx; // Trả về chỉ số của nhiệm vụ được thêm
}

void SCH_Delete_Task(const int task_index) {
    // Xóa task
    SCH_tasks_G[SCH_MAX_TASKS - 1].pTask = NULL;
    SCH_tasks_G[SCH_MAX_TASKS - 1].Delay = 0;
    SCH_tasks_G[SCH_MAX_TASKS - 1].Period = 0;
    SCH_tasks_G[SCH_MAX_TASKS - 1].RunMe = 0;
    SCH_tasks_G[task_index].taskID = 0;
}

void SCH_Dispatch_Tasks(void) {
    for (unsigned char idx = 0; idx < SCH_MAX_TASKS; idx++) {
        if (SCH_tasks_G[idx].RunMe > 0) { // Kiểm tra xem nhiệm vụ có sẵn sàng chạy không
            if (SCH_tasks_G[idx].pTask != NULL) { // Đảm bảo con trỏ nhiệm vụ hợp lệ
                SCH_tasks_G[idx].pTask(); // Thực thi nhiệm vụ
                SCH_tasks_G[idx].RunMe--; // Đặt lại cờ
            }
            if (SCH_tasks_G[idx].Period == 0) { // Nếu là nhiệm vụ chỉ chạy một lần
                SCH_Delete_Task(idx); // Xóa nhiệm vụ
            }
        }
    }
}
