#include "driverlib.h"
#include "device.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <file.h>

#include "F2837xD_sci_io.h"
/*
 * init.c
 *
 *  Created on: 2017-4-16
 *      Author: redchenjs
 */

volatile int scia_status = 0;
volatile FILE *scia_fid;

void system_init(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();


    EINT;
    ERTM;

    //
    // Redirect STDOUT to SCI
    //
    scia_status = add_device("scia", _SSA, SCI_open, SCI_close, SCI_read, SCI_write,
                        SCI_lseek, SCI_unlink, SCI_rename);
    scia_fid = fopen("scia","w");
    freopen("scia:", "w", stdout);
    setvbuf(stdout, NULL, _IONBF, 0);
}

void system_suspend(void)
{
    DEVICE_DELAY_US(100000000);
}
