#ifndef LCD_H
#define LCD_H

/* Copyright 12/8/2023 Szm */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define LCD_PATH	"/dev/fb0"  //指的是LCD的文件路径

//该结构体为了存储LCD相关的硬件信息
typedef struct lcd_info
{
	int fd; 			        //记录LCD的文件描述符
	unsigned int *mp;           //记录LCD的内存映射地址
    unsigned int width;         //lcd的宽
    unsigned int height;        //lcd的高
    int bpp;                    //lcd的色深

}LcdInfo_t;



/*
    函数名称：LCD_Init
    函数功能：对LCD进行初始化
    函数参数：
               @lcdpath：在开发板上的lcd的文件路径
    返回值：成功返回 LcdInfo_t 类型的结构体指针， 失败返回NULL
    函数作者：Szm
    函数版本：V1.0
    创建时间：02/08/2023
    注意事项：该lcd路径是在运行程序的硬件平台的路径
*/
LcdInfo_t * LCD_Init(const char *lcdpath);

#endif // !LCD_H
