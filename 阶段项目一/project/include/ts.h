#ifndef TS_H
#define TS_H

/* Copyright 12/8/2023 Szm */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <linux/input.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define TS_PATH	    "/dev/input/event0"  //指的是触摸屏的文件路径

/*
    函数名称：TS_Init
    函数功能：触摸屏的初始化
    函数参数：
               @tspath：在开发板上的ts的文件路径
    返回值：成功返回触摸屏的文件描述符， 失败返回-1
    函数作者：Szm
    函数版本：V1.0
    创建时间：02/08/2023
    注意事项：该触摸屏路径是在运行程序的硬件平台的路径
*/
int TS_Init(const char *tspath);

/*
    函数名称：TS_GetVal
    函数功能：获取触摸点上的坐标
    函数参数：
               @tspath：在开发板上的ts的文件路径
               @ts_x：触摸点的x轴
               @ts_y：触摸点的y轴
    返回值：成功返回1
    函数作者：Szm
    函数版本：V1.0
    创建时间：02/08/2023
    注意事项：该触摸屏路径是在运行程序的硬件平台的路径
*/
int TS_GetVal(int ts_fd,int *ts_x,int *ts_y);

#endif // !