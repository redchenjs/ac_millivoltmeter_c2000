/*
 * app_adc.c
 *
 *  Created on: 2017-6-27
 *      Author: redchenjs
 */
#include <inc/driver/ssd1351.h>
#include "driverlib.h"
#include "device.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "inc/apps/app_adc.h"
#include "inc/device/adc.h"
#include "inc/system/fonts.h"

extern uint16_t adcAResults[];

double microvolt = 0;
double adc_sum = 0;

int adc_mode = 0;

int16_t  x1_offset = 0;
uint16_t x1_ratio  = 1000;

int16_t  x10_offset = 0;
uint16_t x10_ratio  = 1000;

int16_t  x100_offset = 0;
uint16_t x100_ratio  = 1000;

enum amp_mode {
    AMP_MODE_X1   = 0x00,
    AMP_MODE_X10  = 0x01,
    AMP_MODE_X100 = 0x02
};

void app_adc(void)
{
    uint16_t i;
    char str[20];

    static int above_x1_cnt   = 0;
    static int above_x10_cnt  = 0;
    static int below_x10_cnt  = 0;
    static int below_x100_cnt = 0;

    adc_start();

    for (i=0; i<1024; i++) {
        microvolt = 30000.0 * (2.0 * adcAResults[i] / 65536.0 - 1.0);
        adc_sum += pow(microvolt, 2);
    }

    adc_sum /= 1024.0;

    microvolt = sqrt(adc_sum);

    putchar(0x1B);
    putchar('[');
    putchar('u');

    printf("%lu", (uint32_t)(microvolt));

    switch (adc_mode) {
    case AMP_MODE_X1:
        microvolt *= (x1_ratio / 1000.0);
        microvolt += x1_offset;
        if (microvolt < 1000) {
            if (above_x1_cnt++ >= 5) {
                above_x1_cnt = 0;
                GPIO_writePin(97, 1);
                GPIO_writePin(94, 0);
                adc_mode = AMP_MODE_X10;
                DEVICE_DELAY_US(100000);
            }
        }
        ssd1351_display_string(48, 0, " x 1 ", FONT_1206, Yellow, Blue);
        break;
    case AMP_MODE_X10:
        microvolt *= (x10_ratio / 1000.0);
        microvolt += x10_offset;
        if (microvolt < 1000) {
            if (above_x10_cnt++ >= 5) {
                above_x10_cnt = 0;
                GPIO_writePin(97, 0);
                GPIO_writePin(94, 0);
                adc_mode = AMP_MODE_X100;
                DEVICE_DELAY_US(100000);
            }
        } else if (microvolt > 10000) {
            if (below_x10_cnt++ >= 5) {
                below_x10_cnt = 0;
                GPIO_writePin(97, 1);
                GPIO_writePin(94, 1);
                adc_mode = AMP_MODE_X1;
                DEVICE_DELAY_US(100000);
            }
        }
        ssd1351_display_string(48, 0, " x 10", FONT_1206, Yellow, Blue);
        break;
    case AMP_MODE_X100:
        microvolt *= (x100_ratio / 1000.0);
        microvolt += x100_offset;
        if (microvolt > 10000) {
            if (below_x100_cnt++ >= 5) {
                below_x100_cnt = 0;
                GPIO_writePin(97, 1);
                GPIO_writePin(94, 0);
                adc_mode = AMP_MODE_X10;
                DEVICE_DELAY_US(500000);
            }
        }
        ssd1351_display_string(48, 0, "x 100", FONT_1206, Yellow, Blue);
        break;
    }

    snprintf(str, 10, "%5lu", (uint32_t)(microvolt));
    ssd1351_display_string(16, 46, str, FONT_3216, Magenta, Black);
}
