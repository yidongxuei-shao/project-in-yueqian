/*
	Copyright 12/8/2023 Szm
    lcd配置与控制函数
*/

#include "lcd.h"

/*
    函数名称：LCD_Init
    函数功能：对LCD进行初始化，并获取lcd相关参数
             存储在LcdInfo_t类型结构体中
    函数参数：
               @lcdpath：在开发板上的lcd的文件路径
    返回值：成功返回 LcdInfo_t 类型的结构体指针， 失败返回NULL
    函数作者：Szm
    函数版本：V1.0
    创建时间：02/08/2023
    注意事项：该lcd路径是在运行程序的硬件平台的路径
*/
LcdInfo_t * LCD_Init(const char *lcdpath)
{
	//1.申请堆内存来存储获取的LCD屏幕的参数信息
	LcdInfo_t *lcd = calloc(1,sizeof(LcdInfo_t)); 
	if (NULL == lcd)
	{
		perror("calloc for lcd error");
		return NULL;
	}

	//1.打开LCD，调用open函数实现，由于mmap的时候需要读写权限
	lcd->fd = open(lcdpath,O_RDWR);
	if(-1 == lcd->fd)
	{
		perror("open lcd error");
		return NULL;
	}

    /* 获取lcd屏的宽、高和色深 */
    struct fb_var_screeninfo screeninfo;
    ioctl(lcd->fd, FBIOGET_VSCREENINFO, &screeninfo);

    lcd->width = screeninfo.xres;
    lcd->height = screeninfo.yres;
    lcd->bpp = screeninfo.bits_per_pixel / 8;

	//2.对LCD进行内存映射，调用mmap函数，记得接收函数返回值
	lcd->mp = (unsigned int *)mmap(
									NULL,
									lcd->width * lcd->height * lcd->bpp,		
									PROT_READ|PROT_WRITE,
									MAP_SHARED,
									lcd->fd,
									0
								);
	if (MAP_FAILED == lcd->mp)
	{
		perror("mmap for lcd error");
		return NULL;
	}

	return lcd;
}


