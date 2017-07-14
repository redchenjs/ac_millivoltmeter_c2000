/*
 * task_adc.c
 *
 *  Created on: 2017-6-27
 *      Author: redchenjs
 */
#include "driverlib.h"
#include "device.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "inc/device/adc.h"
#include "inc/system/fonts.h"
#include "inc/driver/ssd1351.h"

extern uint16_t adcAResults[];  // ADC采样结果数组

int16_t  x1_offset = 0;     // x1量程档偏移调整
uint16_t x1_ratio  = 1000;  // x1量程档系数调整

int16_t  x10_offset = 0;
uint16_t x10_ratio  = 1000;

int16_t  x100_offset = 0;
uint16_t x100_ratio  = 1000;

enum amp_mode {
    AMP_MODE_X1   = 0x00,
    AMP_MODE_X10  = 0x01,
    AMP_MODE_X100 = 0x02
};

void task_adc(void)
{
    uint16_t i;
    char str[20];

    double voltage = 0;   // 存储电压值
    double adc_sum = 0;   // 求和变量

    static int adc_mode = 0;    // 存储目前量程
    static int above_x1_cnt   = 0;  // 量程计数
    static int above_x10_cnt  = 0;
    static int below_x10_cnt  = 0;
    static int below_x100_cnt = 0;

    adc_start();    // 启动一次采样

    for (i=0; i<1024; i++) {
        voltage = 30000.0 * (2.0 * adcAResults[i] / 65536.0 - 1.0);   // 计算实际电压
        adc_sum += pow(voltage, 2);   // 平方求和
    }

    adc_sum /= 1024.0;          // 求平均值

    voltage = sqrt(adc_sum);    // 求取均方根值

    printf("%lu\n", (uint32_t)(voltage)); // 串口输出结果用于调试

    switch (adc_mode) {     // 选择量程模式
        case AMP_MODE_X1:   // x1量程档
            voltage *= (x1_ratio / 1000.0); // 系数修正
            voltage += x1_offset;           // 偏移修正
            if (voltage < 1000) {           // 欠压升档
                if (above_x1_cnt++ >= 5) {  // 多次测量防止抖动
                    above_x1_cnt = 0;       // 清计数值
                    GPIO_writePin(97, 1);   // 第一级放大倍数设为1倍
                    GPIO_writePin(94, 0);   // 第二级放大倍数设为10倍
                    adc_mode = AMP_MODE_X10;// 更新量程
                    DEVICE_DELAY_US(100000);// 延时测量防止抖动
                }
            }
            ssd1351_display_string(48, 0, " x 1 ", FONT_1206, Yellow, Blue);// 更新显示
            break;
        case AMP_MODE_X10:  // x10量程档
            voltage *= (x10_ratio / 1000.0);
            voltage += x10_offset;
            if (voltage < 1000) {           // 欠压升档
                if (above_x10_cnt++ >= 5) {
                    above_x10_cnt = 0;
                    GPIO_writePin(97, 0);   // 第一级放大倍数设为10倍
                    GPIO_writePin(94, 0);   // 第二级放大倍数设为10倍
                    adc_mode = AMP_MODE_X100;
                    DEVICE_DELAY_US(100000);
                }
            } else if (voltage > 10000) {   // 超压降档
                if (below_x10_cnt++ >= 5) {
                    below_x10_cnt = 0;
                    GPIO_writePin(97, 1);   // 第一级放大倍数设为1倍
                    GPIO_writePin(94, 1);   // 第二级放大倍数设为1倍
                    adc_mode = AMP_MODE_X1;
                    DEVICE_DELAY_US(100000);
                }
            }
            ssd1351_display_string(48, 0, " x 10", FONT_1206, Yellow, Blue);
            break;
        case AMP_MODE_X100:
            voltage *= (x100_ratio / 1000.0);
            voltage += x100_offset;
            if (voltage > 10000) {          // 超压降档
                if (below_x100_cnt++ >= 5) {
                    below_x100_cnt = 0;
                    GPIO_writePin(97, 1);   // 第一级放大倍数设为1倍
                    GPIO_writePin(94, 0);   // 第二级放大倍数设为10倍
                    adc_mode = AMP_MODE_X10;
                    DEVICE_DELAY_US(500000);
                }
            }
            ssd1351_display_string(48, 0, "x 100", FONT_1206, Yellow, Blue);
            break;
        default:
            break;
    }

    snprintf(str, 10, "%5lu", (uint32_t)(voltage));     // 格式化电压值
    ssd1351_display_string(16, 46, str, FONT_3216, Magenta, Black); //更新显示
}
