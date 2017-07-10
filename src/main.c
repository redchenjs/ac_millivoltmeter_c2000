#include "inc/system/init.h"
#include "inc/system/user.h"
/*
 * main.c
 *
 *  Created on: 2017-4-16
 *      Author: redchenjs
 */

int main(void)
{
    system_init();

    user_init();

    while (1) {
        user_loop();

        system_suspend();
    }
}
