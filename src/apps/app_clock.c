#include <inc/system/fonts.h>
#include <inc/driver/ssd1351.h>
#include "driverlib.h"
#include "device.h"
#include <stdio.h>
/*
 * app_clock.c
 *
 *  Created on: 2017年7月6日
 *      Author: redchenjs
 */
extern uint16_t cpuTimer0IntCount;

void app_clock(void)
{
    char tim[20] = {0};

    snprintf(tim, 20, "%02u:%02u:%02u", (cpuTimer0IntCount/60/60)%24, (cpuTimer0IntCount/60)%60, cpuTimer0IntCount%60);
    ssd1351_display_string(0, 92, tim, FONT_3216, Lime, Black);
}

