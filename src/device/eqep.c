#include <driverlib.h>
#include <device.h>
/*
 * eqep.c
 *
 *  Created on: 2017-7-6
 *      Author: redchenjs
 */

void eqep3_init(void)
{
    GPIO_setPinConfig(GPIO_6_EQEP3A);
    GPIO_setPadConfig(6, GPIO_PIN_TYPE_STD);

    //
    // Configure the decoder for up-count mode, counting both rising and
    // falling edges (that is, 2x resolution)
    //
    EQEP_setDecoderConfig(EQEP3_BASE, (EQEP_CONFIG_2X_RESOLUTION |
                                       EQEP_CONFIG_UP_COUNT |
                                       EQEP_CONFIG_NO_SWAP));
    EQEP_setEmulationMode(EQEP3_BASE, EQEP_EMULATIONMODE_RUNFREE);

    //
    // Configure the position counter to reset on an index event
    //
    EQEP_setPositionCounterConfig(EQEP3_BASE, EQEP_POSITION_RESET_UNIT_TIME_OUT,
                                  0xFFFFFFFF);

    //
    // Enable the unit timer, setting the frequency to 100 Hz
    //
    EQEP_enableUnitTimer(EQEP3_BASE, DEVICE_SYSCLK_FREQ);

    //
    // Configure the position counter to be latched on a unit time out
    //
    EQEP_setLatchMode(EQEP3_BASE, EQEP_LATCH_UNIT_TIME_OUT);

    //
    // Enable the eQEP1 module
    //
    EQEP_enableModule(EQEP3_BASE);

    //
    // Configure and enable the edge-capture unit. The capture clock divider is
    // SYSCLKOUT/128. The unit-position event divider is QCLK/8.
    //
    EQEP_setCaptureConfig(EQEP3_BASE, EQEP_CAPTURE_CLK_DIV_128,
                          EQEP_UNIT_POS_EVNT_DIV_8);
    EQEP_enableCapture(EQEP3_BASE);
}
