/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-09     张JASON       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include <rtdbg.h>
#include <led.h>

/*宏定义*/
#define LED0_PIN  GET_PIN(B,3)

#define LED_THREAD_PRIORITY         25
#define LED_THREAD_STACK_SIZE       512
#define LED_THREAD_TIMESLICE        5

/*定义线程控制块*/
rt_thread_t led1_thread = RT_NULL;                                  //创建线程控制块

/*函数声明*/
static void led_control_thread_entry(void *parameter);


/* 创建线程 */
int led_control(void)
{
    /* 创建线程*/
    led1_thread = rt_thread_create("led_control_thread",                        //给线程取的名字
                            led_control_thread_entry,                    //线程函数
                            RT_NULL,                                       //线程函数的参数
                            LED_THREAD_STACK_SIZE,                             //分配给线程的栈空间的大小
                            LED_THREAD_PRIORITY,                               //线程的优先级
                            LED_THREAD_TIMESLICE);                             //时间片？

    /* 如果获得线程控制块,启动线程,开启调度 */
    if (led1_thread != RT_NULL)
        rt_thread_startup(led1_thread);

    return 0;
}
//MSH_CMD_EXPORT(led_control_thread_entry, Led twinkle );

/* 入口函数 */
static void led_control_thread_entry(void *parameter)
{
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    while(1){
//    rt_pin_write(LED0_PIN, 1);
//    rt_kprintf("led_off\n");
    rt_thread_mdelay(1000);

    //rt_pin_write(LED0_PIN, 0);
    //rt_kprintf("led_on\n");
//    rt_thread_mdelay(1000);
    }
}



