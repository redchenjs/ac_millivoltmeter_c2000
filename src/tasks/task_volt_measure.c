#include <inc/apps/app_adc.h>
#include <inc/apps/app_freq.h>
#include <inc/apps/app_clock.h>

#include "inc/device/spi.h"
#include "inc/device/adc.h"
#include "inc/device/gpio.h"
#include "inc/device/eqep.h"
#include "inc/device/cpu_timer.h"

#include <inc/driver/ssd1351.h>
/*
 * task_volt_measure.c
 *
 *  Created on: 2017-7-7
 *      Author: redchenjs
 */

void task_volt_measure_run(void)
{
    app_freq();
    app_adc();
    app_clock();
}

void task_volt_measure_init(void)
{
    spi_init();
    adc_init();
    gpio_init();
    eqep3_init();
    cpu_timer_init();

    ssd1351_init();


}
