#ifndef _BMP_H
#define _BMP_H


/*************************************
 * 
 *              bmp.h
 * 
 * ***********************************/
#include <stdio.h>
#include <stdbool.h>
#include "lcd.h"
/**************************************
 * 
 *                定义
 * 
 * ************************************/

int bmp_show(lcd_info_t* lcd_info, const char* file_name, int start_x, int start_y);

int bmp_show_1(lcd_info_t* lcd_info);
/****************************************
 * 
 *            BMP图像路径
 * 
 * **************************************/
#define BMP_PATH_1 "/home/shao/nfs/learn/03_project/image/picture/10.bmp"

#define BMP_PATH_2 "/home/shao/nfs/learn/03_project/image/picture/7.bmp"

#endif // !_BMP_H