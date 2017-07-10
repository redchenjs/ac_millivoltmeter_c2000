#include <inc/tasks/task_volt_measure.h>
/*
 * user.c
 *
 *  Created on: 2017-4-16
 *      Author: redchenjs
 */

void user_loop(void)
{
    task_volt_measure_run();
}

void user_init(void)
{
    task_volt_measure_init();
}
