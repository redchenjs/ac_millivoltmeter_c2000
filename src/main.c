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
    system_init();          //系统初始化

    user_init();            //应用初始化

    while (1) {
        user_loop();        //应用循环

        system_suspend();           //系统暂停
    }
}
