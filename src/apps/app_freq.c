#include <driverlib.h>
#include <device.h>
#include <stdio.h>
#include <inc/system/fonts.h>
#include <inc/driver/ssd1351.h>
/*
 * app_freq.c
 *
 *  Created on: 2017年7月9日
 *      Author: redchenjs
 */

uint32_t intFreq = 5000;
uint32_t intPeriod = 0x07A1;
void app_freq(void)
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
        intFreq = EQEP_getPositionLatch(EQEP3_BASE) * 100 / 2.0;
        EQEP_clearInterruptStatus(EQEP3_BASE, EQEP_INT_UNIT_TIME_OUT);
    }
    char cnt[20] = {0};

    snprintf(cnt, 20, "%05lu", intFreq);
    if (intFreq != 0) {
        intPeriod = 200000000 / (intFreq / 100) / 2 / 1024;
        EPWM_disableADCTrigger(EPWM1_BASE, EPWM_SOC_A);
        EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
        EPWM_setTimeBasePeriod(EPWM1_BASE, intPeriod);
    }
    ssd1351_display_string(48, 20, cnt, FONT_1206, Snow, Black);
}
