/*
 * user.c
 *
 *  Created on: 2017-4-16
 *      Author: redchenjs
 */
#include <inc/tasks/task_adc.h>
#include <inc/tasks/task_freq.h>
#include <inc/tasks/task_clock.h>

void user_loop(void)
{
    task_freq();    // 任务一：频率测量
    task_adc();     // 任务二：A/D采样
    task_clock();   // 任务三：时钟显示
}

void user_init(void)
{
// 用户程序初始化函数，暂时为空
}
