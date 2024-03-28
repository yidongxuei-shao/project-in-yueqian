#ifndef _READ_JPEG_H
#define _READ_JPEG_H
/************************************************
 * 
 *                  头文件
 * 
 * *********************************************/
#include <linux/fb.h>
#include <stdio.h>
#include "../include/jpeglib.h"
#include <stdlib.h>
#include <setjmp.h>
#include "dir_scan.h"
#include "lcd.h"
/************************************************
 * 
 *                  函数声明
 * 
 * *********************************************/
int read_JPEG_file (const char * filename, int* lcd_map, int start_x, int start_y);
// int read_JPEG_file(char * filename,unsigned int * lcd_mp,int start_x,int start_y);
int show_GIF(lcd_info_t *lcd_info ,const char *dir_name, 
                    int start_x, int start_y, int speed);
int show_picture(lcd_info_t *lcd_info ,const char *picture_path, int rate,
                    int start_x, int start_y);

/************************************************
 * 
 *                  文件路径
 * 
 * **********************************************/
#define GIF_PATH         "/home/shao/nfs/learn/03_project/image/GIF/hutao"
#define JPEG_PATH        "/home/shao/nfs/learn/03_project/image/picture/logo.jpg"

#endif // !_READ_JPEG_H

