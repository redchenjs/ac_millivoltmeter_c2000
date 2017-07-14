/*
 * task_clock.c
 *
 *  Created on: 2017-7-6
 *      Author: redchenjs
 */
#include <stdio.h>

#include "driverlib.h"
#include "device.h"

#include "inc/system/fonts.h"
#include "inc/driver/ssd1351.h"

extern uint16_t cpuTimer0IntCount;  // CPU Timer 0 计数变量

void task_clock(void)
{
    char tim[20] = {0};

    // 格式化处理计数变量， 显示上电以来系统运行的时间
    snprintf(tim, 20, "%02u:%02u:%02u", (cpuTimer0IntCount/60/60)%24, (cpuTimer0IntCount/60)%60, cpuTimer0IntCount%60);
    // 更新显示
    ssd1351_display_string(0, 92, tim, FONT_3216, Lime, Black);
}

