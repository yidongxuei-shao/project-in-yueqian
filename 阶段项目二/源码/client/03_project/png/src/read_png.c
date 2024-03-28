#include "read_png.h"

/* 
   Copyright 12/8/2023 Szm
   显示PNG文件
*/
/*
    x --> x轴的偏移值
    y --> y轴的偏移值
*/
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
int read_png_file(unsigned int x, unsigned int y, const char* filename, lcd_info_t* lcd) 
{

   FILE *fp;                  /* 要打开的文件流指针 */
   png_structp png_ptr;       /* png结构体 */
   png_infop info_ptr;        /* 图片信息结构体 */
   png_uint_32 width, height; /* 图片的宽和高 */

   /* 图片信息 */
   int bit_depth, color_type, interlace_type, number_passes;
    
   u32 b_rgb24;     /* 当前屏幕上的RGB数据，要将它和读取出来的png数据进行混合，得到有透明背景RGB数据 */
   u8 r,g,b,a;      /* 图片RGB数据 */
   u8 b_r,b_g,b_b;   /* 当前屏幕上的RGB数据 */
   u8 R,G,B;         /* 合成后的RGB数据 */
   
   if((fp = fopen(filename,"rb")) == NULL)
   {
   printf("%s can not open\r\n",filename);
   return -1;
   }

    /* 创建png读取结构体 */
   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);

    /* 需要分配/初始化内存以获取图像信息 */
   info_ptr = png_create_info_struct(png_ptr);

   /*设置PNG图片的文件指针*/
    png_init_io(png_ptr,fp);     /* 将png结构体与文件相连 */

   png_read_info(png_ptr,info_ptr);    /* 读取png文件文件 */

   /* 获取png文件的信息 */
   png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, 
                     &color_type,&interlace_type, NULL, NULL);

   // printf("picture width = %d\n",width);
   // printf("picture height =%d\n",height);
   // printf("picture bit_depth = %d\n",bit_depth);
   
    /*读取图像的最简单方法：*/
    png_bytep row_pointers[height];    /* 定义一个变量用于存储整个png图片的RGBA数据 */

   /* 申请堆内存 */
   for(int i = 0; i < height; i++)
   {
      row_pointers[i] = (unsigned char*)calloc(width * 4, 1); /* RGBA */
      if(row_pointers[i] == NULL)
      {
         perror("can not calloc memory for png");
         return -1;
      }
   }

   /* 读取整个PNG图像 */
   png_read_image(png_ptr,row_pointers);

   u32 x0, x1;

    for(int i = 0; i < height; i++)
    {
      x0 = x;
      x1 = 0;
      for(int j = 0; j < width * 4; j += 4)
      {
         /*得到图片颜色*/
         r=row_pointers[i][j + 0];
         g=row_pointers[i][j + 1];
         b=row_pointers[i][j + 2];
         a=row_pointers[i][j + 3];
         
         /* 读取当前屏幕点的背景颜色 */

         b_rgb24=LCD_ReadPoint(x0, y, lcd);

         /* 通过位右移， 获得当前屏幕上的RGB颜色 */
         b_r=b_rgb24>>16 & 0xFF;      /* 红 */
         b_g=b_rgb24>>8 & 0xFF;       /* 绿 */
         b_b=b_rgb24>>0 & 0xFF;       /* 蓝 */
         
         /* 混合颜色，将从图片读取到的RGBA与当前屏幕的RGB颜色数据进行混合，得到透明背景的图片 */
         R = (unsigned char)(r * (a / 255.0) + (b_r * (255 - a)) / 255.0);
         G = (unsigned char)(g * (a / 255.0) + (b_g * (255 - a)) / 255.0);
         B = (unsigned char)(b * (a / 255.0) + (b_b * (255 - a)) / 255.0);
      
         /* 显示数据 */
         /* 写入lcd屏中 */
         lcd->lcd_mp[lcd->widch * (y - 1) + (x1 + x)] = R<<16 | G<<8 | B;
         
         /*坐标自增*/
         x0++;
         x1++;
      }

      y++;  /* 移动下一行 */
   }
    /* 读取文件的其余部分，并在info_ptr中获取其他块-必需*/
    png_read_end(png_ptr, info_ptr);
   /*读取后清理，并释放已分配的所有内存-必需*/
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    /* 统一释放内存 */
   for(int i = 0; i < height; i++)
   {
      free(row_pointers[i]);
   }
    /*关闭文件*/
    fclose(fp);
    return 0;
}

/*
函数功能: 读点函数
函数参数： u32 x,u32 y,u16 c
*/

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
u32 LCD_ReadPoint(u32 x,u32 y, lcd_info_t* lcd_info)
{
   unsigned int * lcd_cur_point = lcd_info->lcd_mp + lcd_info->widch * y + x;
   return *lcd_cur_point;
}


