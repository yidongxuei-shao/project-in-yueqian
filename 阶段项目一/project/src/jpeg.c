/*
    Copyright 12/8/2023 Szm
    该文件是读取jpeg格式图片
    并将图片通过地址映射写到lcd屏幕上
*/

#include "jpeg.h"
#include "double_list.h"


//显示一张jpg图像

/*
    函数名称：read_JPEG_file
    函数功能：读取jpeg格式图片并将其显示在lcd屏幕上
    函数参数：
                @start_x：图片在x轴的偏移值
                @start_y：图片在y轴的偏移值
                @filename:图片文件的路径
                @lcd：    显示图片lcd的参数，
                        具体看lcd.h
    返回值：    None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int read_JPEG_file (int start_x, int start_y, char * filename, LcdInfo_t * lcd)
{
 
    struct jpeg_decompress_struct cinfo;    /* 定义结构体变量用于存储图片信息 */
    
    struct jpeg_error_mgr jerr;             /* 错误处理 */

    /* More stuff */
    FILE * infile;		            /* 文件流指针 */
    unsigned char * buffer;         //为了存储申请的内存地址
    int row_stride;		            // 为了存储计算的图像一行大小

    /* 打开文件 */
    if ((infile = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
        return 0;
    }

    /* 设置错误处理 */
    cinfo.err = jpeg_std_error(&jerr);
    
    /* 创建解压对象 */
    jpeg_create_decompress(&cinfo);

    /* 将解压对象与解压文件（图片）结合 */
    jpeg_stdio_src(&cinfo, infile);

    /* 读取解压文件 */
    jpeg_read_header(&cinfo, TRUE);
    
    /* 开始解压 */
    jpeg_start_decompress(&cinfo);
    
    //计算图像一行大小的缓冲区，准备来存储读取出来的颜色分量
    row_stride = cinfo.output_width * cinfo.output_components;  

    //申请了图像一行宽度大小的堆内存来存储读取的颜色分量
    buffer = (unsigned char *) calloc(row_stride,1);  

    
    while (cinfo.output_scanline < cinfo.output_height) 
    {
        //已经把图像的一行像素颜色分量读取出来并存储在了堆内存中  jpg图片的像素点色深为24bit 
        //另外jpg图片的像素颜色分量 RGB
        jpeg_read_scanlines(&cinfo, &buffer, 1); 
        
        //把读取出来的图像颜色分量写入到LCD对应的内存空间
        for (int i = 0; i < cinfo.output_width; ++i)
        {
            *(lcd->mp + start_x + i + (cinfo.output_scanline -1 + start_y)*800 ) = 
                        (buffer[i*3]<<16) | (buffer[i*3+1]<<8) | buffer[i*3+2]; //RGB RGB RGB RGB ....
        }
    }

    /* 结束解压 */
    jpeg_finish_decompress(&cinfo);
    
    /* 释放内存 */
    jpeg_destroy_decompress(&cinfo);

    /* 关闭文件 */
    fclose(infile);

    return 1;
}

/* 显示GIF图片 */
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
                 int color, int x, int y)
{
	char gif_name[256] = {0}; //为了存储GIF图像的名称

    int count = get_picture_info(NULL, gifpath);
    /* 清屏 */
    memset(lcd->mp, color, lcd->height*lcd->width*lcd->bpp);


	for (int i = 0; i < count; ++i)
	{
        //构造一帧图像的路径
		sprintf(gif_name,"%s/Frame%d.jpg",gifpath,i); 
		read_JPEG_file (x, y, gif_name,lcd); //显示jpg图片

		usleep(speed * 1000);   //延时speedms
	}
}