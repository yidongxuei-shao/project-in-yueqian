#ifndef READ_PNG_H
#define READ_PNG_H

/* Copyright 12/8/2023 Szm */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <linux/videodev2.h>
#include <poll.h>
#include <png.h>
#include <pngconf.h>

#include "lcd.h"

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;


/*
    函数名称：read_png_file
    函数功能：在lcd上显示一张png格式的图片，带有透明背景处理
    函数参数：
               @x : 图片在lcd上的x轴偏移值
               @y : 图片在lcd上的y轴偏移值
               @filename：图片的路径
               @lcd：lcd的参数信息
    返回值：成功返回0， 失败返回-1
    函数作者：Szm
    函数版本：V1.0
    创建时间：08/08/2023
    注意事项：图片大小不要超过lcd的大小，没有相应的边界处理
*/
int read_png_file(unsigned int x, unsigned int y, const char* filename, LcdInfo_t* lcd);


/*
    函数名称：LCD_ReadPoint
    函数功能：读取lcd指定的像素信息
    函数参数：
               @x : 图片在lcd上的x轴偏移值
               @y : 图片在lcd上的y轴偏移值
               @lcd_info：lcd的参数信息
    返回值：成功返回lcd的像素信息
    函数作者：Szm
    函数版本：V1.0
    创建时间：08/08/2023
*/
u32 LCD_ReadPoint(u32 x,u32 y, LcdInfo_t* lcd_info);


#endif // !READ_PNG_H