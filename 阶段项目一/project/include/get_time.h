#ifndef GET_TIME_H
#define GET_TIME_H

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "double_list.h"


/* Copyright 12/8/2023 Szm */
#define TIME_START      1
#define TIME_END        2

#define  TIME_LONG      25


typedef struct get_time
{
    char time_all[50];
    unsigned int hour;      /* 时 */
    unsigned int min;       /* 分 */
    unsigned int sec;       /* 秒 */

}get_time_t;


/*
    函数名称：get_time
    函数功能：获取当前的系统时间，返回get_time_t结构体指针
    函数参数：None
    返回值：    成功返回get_time_t结构体指针，失败返回NULL
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
get_time_t* get_time(void);


/*
    函数名称：show_time
    函数功能：打印输入time内的数据
    函数参数：
                @time：存储时间结构体指针
    返回值：    None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
void show_time(get_time_t *time);


/*
    函数名称：get_time_devi
    函数功能：从时间字符串分割出，时分秒，存储在get_time_t结构体中
    函数参数：
                @time：时间字符串
                @time_data：分割结束后的存储指针
    返回值：    None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
void get_time_devi(char* time, get_time_t *time_data);

#endif // !