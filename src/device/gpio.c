/*
 * gpio.c
 *
 *  Created on: 2017-7-7
 *      Author: redchenjs
 */
#include "driverlib.h"
#include "device.h"

void gpio_init(void)
{
    GPIO_setPadConfig(97U, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_97_GPIO97);
    GPIO_setDirectionMode(97U, GPIO_DIR_MODE_OUT);
    GPIO_writePin(97U, 1);

    GPIO_setPadConfig(94U, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_94_GPIO94);
    GPIO_setDirectionMode(94U, GPIO_DIR_MODE_OUT);
    GPIO_writePin(94U, 1);
}
