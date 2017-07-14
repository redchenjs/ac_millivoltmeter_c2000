/*
 * task_freq.c
 *
 *  Created on: 2017-7-9
 *      Author: redchenjs
 */
#include <driverlib.h>
#include <device.h>
#include <stdio.h>
#include <inc/system/fonts.h>
#include <inc/driver/ssd1351.h>

uint32_t intFreq = 5000;        // 初始频率为50Hz
uint32_t intPeriod = 0x07A1;    // 初始定时器周期

void task_freq(void)
{
    //
    // **** Frequency calculation using eQEP position counter ****
    //
    //  Check for unit time out event
    //
    if((EQEP_getInterruptStatus(EQEP3_BASE) & EQEP_INT_UNIT_TIME_OUT) != 0)
    {
        //
        // Get latched POSCNT value
        //
        intFreq = EQEP_getPositionLatch(EQEP3_BASE) * 100 / 2.0;        // 计算频率
        EQEP_clearInterruptStatus(EQEP3_BASE, EQEP_INT_UNIT_TIME_OUT);  // 清除中断标志
    }
    char cnt[20] = {0};

    snprintf(cnt, 20, "%05lu", intFreq);    // 格式化处理频率值
    if (intFreq != 0) {                                         // 如果成功测到频率
        intPeriod = 200000000 / (intFreq / 100) / 2 / 1024;     // 计算ePWM定时器周期
        EPWM_disableADCTrigger(EPWM1_BASE, EPWM_SOC_A);         // 禁用ADC触发器
        EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_STOP_FREEZE); // 停止ePWM模块的TB定时器
        EPWM_setTimeBasePeriod(EPWM1_BASE, intPeriod);          // 重设定时器周期
    }
    ssd1351_display_string(48, 20, cnt, FONT_1206, Snow, Black);    // 更新显示
}
