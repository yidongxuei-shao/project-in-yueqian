#ifndef _LCD_H
#define _LCD_H

/***********************************************
 * 
 *                  头文件
 * 
 * *********************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

/***********************************************
 * 
 *              lcd信息结构体
 * 
 * *******************************************/
typedef struct lcd_info
{
    int lcd_fd;
    int* lcd_mp;
    int height;
    int widch;
    int colour_deep;
}lcd_info_t;

/**********************************************
 * 
 *               lcd路径
 * 
 * *******************************************/
#define LCD_PATH        "/dev/fb0"

/*********************************************
 * 
 *              函数声明
 * 
 * ******************************************/
lcd_info_t* lcd_init(void);
void lcd_destroy(lcd_info_t * lcd_info);

#endif // !_LCD_H

