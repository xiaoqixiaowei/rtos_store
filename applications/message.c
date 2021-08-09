/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-09     张JASON       the first version
 */
#include <message.h>

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include <rtdbg.h>

#define KEY0_PIN  GET_PIN(A,0)
#define KEY1_PIN  GET_PIN(A,1)

static rt_thread_t receive_message_thread = RT_NULL;
static rt_thread_t send_message_thread = RT_NULL;  //创建线程控制块

/* 定义消息队列控制块 */
static rt_mq_t test_mq = RT_NULL;

/* 入口函数 */
static void message_receive_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    uint32_t r_queue;
    /* 线程都是一个无限循环，不能返回 */
    while (1) {
    /* 队列读取（接收），等待时间为一直等待 */
    uwRet = rt_mq_recv(test_mq, /* 读取（接收）队列的 ID(句柄) */
    &r_queue, /* 读取（接收）的数据保存位置 */
    sizeof(r_queue), /* 读取（接收）的数据的长度 */
    RT_WAITING_FOREVER); /* 等待时间：一直等 */
    if (RT_EOK == uwRet) {
        rt_kprintf("The message receive:%d\n",r_queue);
    }
    else {
        rt_kprintf("receive error \n");
    }
        rt_thread_delay(200);
    }
}

static void message_send_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    uint32_t send_data1 = 1;
    uint32_t send_data2 = 2;
    while (1) {
        /* K1 被按下 */
         if (rt_pin_read(KEY0_PIN)==PIN_LOW) {
         /* 将数据写入（发送）到队列中，等待时间为 0 */
             while(rt_pin_read(KEY0_PIN)==PIN_LOW);
             uwRet = rt_mq_send(test_mq,/* 写入（发送）队列的 ID(句柄) */
                     &send_data1,/* 写入（发送）的数据 */
                     sizeof(send_data1)); /* 数据的长度 */
             if (RT_EOK != uwRet) {
                 rt_kprintf("Send error!\n",uwRet);
             }
         }
         /* K2 被按下 */
        if (rt_pin_read(KEY1_PIN)==PIN_LOW) {
         /* 将数据写入（发送）到队列中，等待时间为 0 */
            while(rt_pin_read(KEY1_PIN)==PIN_LOW);
            uwRet = rt_mq_send(test_mq, /* 写入（发送）队列的 ID(句柄) */
                    &send_data2, /* 写入（发送）的数据 */
                    sizeof(send_data2)); /* 数据的长度 */
            if (RT_EOK != uwRet) {
                rt_kprintf("Send error!\n",uwRet);
            }
        }
        rt_thread_delay(20);
    }
}

/* 创建线程 */
int message_send(void)
{
    /*创建消息队列*/
    test_mq=rt_mq_create("test_mq", 40,20, RT_IPC_FLAG_FIFO);
    if(test_mq!=RT_NULL)
    rt_kprintf("test_message_queue create successfully!");



    /* 创建接收线程*/
    receive_message_thread = rt_thread_create("receive message_thread",                        //给线程取的名字
            message_receive_thread_entry,                    //线程函数
                            RT_NULL,                                       //线程函数的参数
                            512,                             //分配给线程的栈空间的大小
                            2,                               //线程的优先级
                            20);                             //时间片？

    /* 如果获得线程控制块，启动这个线程 */
    if (receive_message_thread != RT_NULL)
        rt_thread_startup(receive_message_thread);
    else {
        return -1;
    }

    /* 创建发送线程*/
    send_message_thread = rt_thread_create("send message_thread",                        //给线程取的名字
                            message_send_thread_entry,                    //线程函数
                            RT_NULL,                                       //线程函数的参数
                            512,                             //分配给线程的栈空间的大小
                            3,                               //线程的优先级
                            20);                             //时间片？

    /* 如果获得线程控制块，启动这个线程 */
    if (send_message_thread != RT_NULL)
        rt_thread_startup(send_message_thread);
    else {
        return -1;
    }



    return 0;
}









