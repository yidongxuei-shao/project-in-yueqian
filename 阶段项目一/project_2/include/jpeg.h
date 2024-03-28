#ifndef JPEG_H
#define JPEG_H

/* Copyright 12/8/2023 Szm */
#include "lcd.h"
#include "jpeglib.h"
#include <dirent.h>

/*
    函数名称：read_JPEG_file
    函数功能：在lcd屏指定的位置显示一张.jpeg格式的图片
    函数参数：
                @start_x：指定位置的x轴坐标
                @start_y：指定位置的y轴坐标
                @filename：要打开的文件名称
                @lcd：自定义结构体存储lcd的参数
    返回值：成功返回1， 失败返回0
    函数作者：Szm
    函数版本：V1.0
    创建时间：02/08/2023
    注意事项：显示的图片的像素必须小于或等于800*480
             指定的x坐标在0~（800 - 像素的宽）
             指定的y坐标在0~（480 - 像素的宽）
*/
int read_JPEG_file (int start_x,int start_y,char * filename,LcdInfo_t * lcd); 



/*
    函数名称：GIF_Show
    函数功能：在lcd上显示GIF格式的文件
    函数参数： 
                @gifpath: 图片文件的路径
                @lcd：    显示图片lcd的参数，
                            具体看lcd.h
                @speed：展示每帧图片的延时时间
                @color：清屏颜色
                @x：图片在x轴的偏移值
                @y：图片在y轴的偏移值
    返回值：    None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
void  GIF_Show(const char *gifpath, LcdInfo_t * lcd, int speed,
                 int color, int x, int y);
#endif // !