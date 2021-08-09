/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-09     张JASON       the first version
 */
/*包含头文件*/
#include <key.h>
#include <led.h>

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include <rtdbg.h>



/*变量*/
#define KEY0_PIN  GET_PIN(A,0)
#define KEY1_PIN  GET_PIN(A,1)

/*定义线程控制块*/
static rt_thread_t key_thread=RT_NULL;
extern rt_thread_t led1_thread;                                  //创建线程控制块

/*函数声明*/
static void key_thread_entry(void* parameter);

/*初始化线程函数,用于main中调用 */
int key_control(void)
{
    rt_kprintf("An example of key and led!\n");

    key_thread=
        rt_thread_create("key_control", /* 线程名字 */
                key_thread_entry,/*线程入口函数*/
                RT_NULL, /*入口函数参数*/
                512, /*线程栈的大小*/
                20, /*线程的优先级*/
                20);/*线程时间片*/
    /*启动线程,开启调度*/
    if(key_thread!=RT_NULL)
        rt_thread_startup(key_thread);
    return 0;
}

void key_Init(void)
{
    rt_pin_mode(KEY0_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY1_PIN, PIN_MODE_INPUT_PULLUP);
}
static void key_thread_entry(void* parameter)
{

    rt_err_t uwRet=RT_EOK;
    rt_kprintf("start\n");
    while(1)
    {
        if(rt_pin_read(KEY0_PIN)==PIN_LOW)
        {
            while(rt_pin_read(KEY0_PIN)==PIN_LOW);
            rt_kprintf("Suspend led1_thread progressing! \n");
            uwRet=rt_thread_suspend(led1_thread);/*挂起LED1线程*/
            if (RT_EOK == uwRet)
            {
                rt_kprintf("Suspend led1_thread Success!\n");
            }
            else {
                rt_kprintf("Suspend led1_thread Fail！\n");
            }
        }
        if(rt_pin_read(KEY1_PIN)==PIN_LOW)
        {
            while(rt_pin_read(KEY1_PIN)==PIN_LOW);
            rt_kprintf("resume led1_thread progressing! \n");
            uwRet=rt_thread_resume(led1_thread);/*恢复LED1线程*/
            if (RT_EOK == uwRet)
            {
                rt_kprintf("resume led1_thread Success!\n");
            }
            else
            {
                rt_kprintf("Suspend led1_thread fail！\n");
            }
        }
        rt_thread_delay(20);
    }
}







