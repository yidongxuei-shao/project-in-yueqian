#ifndef __font_h__
#define __font_h__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

/******************
 *  
 *      我的头文件
 * 
 * ****************/
#include "lcd.h"

#define color u32
#define getColor(a, b, c, d) (a|b<<8|c<<16|d<<24)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef struct stbtt_fontinfo
{
   void           * userdata;
   unsigned char  * data;              // pointer to .ttf file
   int              fontstart;         // offset of start of font

   int numGlyphs;                     // number of glyphs, needed for range checking

   int loca,head,glyf,hhea,hmtx,kern; // table locations as offset from start of .ttf
   int index_map;                     // a cmap mapping for our chosen character encoding
   int indexToLocFormat;              // format needed to map from glyph index to glyph
} stbtt_fontinfo;


typedef struct{
	u32 height;
	u32 width;
	u32 byteperpixel;
	u8 *map;
}bitmap;

typedef struct{
	stbtt_fontinfo *info;
	u8 *buffer;
	float scale;
}font;


//lcd设备结构体
struct LcdDevice
{
	int fd;
	unsigned int *mp; //保存映射首地址

};

/*********************************************
 * 
 *                颜色
 *  
 * ******************************************/
#define BLACK       getColor(0,0,0,0)
#define WHITE       getColor(0,255,255,255)
#define RED         getColor(0,255,0,0)
#define GREEN       getColor(0,0,255,0)
#define BLUE        getColor(0,0,0,255)
#define YELLOW      getColor(0,255,255,0)


/*******************************
 *          NOTE
 *  注意：颜色的设置要用  getColor(A,B,G,R)
 * *****************************/

//1.初始化字库
/**********************************
 * 初始化字库
 * 参数一：DroidSansFallback.ttf 字库的路径 
 * 返回值：操作字库的句柄 
 * *******************************/
font *fontLoad(char *fontPath);

//2.设置字体的大小 
/**********************************
 *  设置字体的大小
 *  参数一：操作的字库 
 *	参数二：字体的大小  
 * *******************************/
void fontSetSize(font *f, s32 pixels);

//3.设置字体输出框的大小
/*********************************
 * 
 *  参数一：输出框的宽 
 *  参数一：输出框的高
 *  参数三：当前屏幕的色素    
 *  例如：32位 -> 4 
 * 
 * ******************************/
bitmap *createBitmap(u32 width, u32 height, u32 byteperpixel);

//可以指定输出框的颜色
/*********************************
 * 
 *  参数一：输出框的宽 
 *  参数一：输出框的高
 *  参数三：当前屏幕的色素
 *  参数四：输出框的颜色    
 *  例如：32位 -> 4 
 * 
 * ******************************/
bitmap *createBitmapWithInit(u32 width, u32 height, u32 byteperpixel, color c);

//4.把字体输出到输出框中
/*********************************
 * 
 *  参数一：操作的字库 
 *  参数二：输出框   
 *  参数三：字体的位置    X轴  
 *  参数四：字体的位置    Y轴
 *  参数五：字体文本  
 *  参数六：字体颜色    
 *  参数七：一行显示字体的最大宽度
 * （例如：字体大小为32，假设设置为64则一行显示两个字体，就换行显示）默认为： 0
 * 
 * ******************************/
void fontPrint(font *f, bitmap *screen, s32 x, s32 y, char *text, color c, s32 maxWidth);

//5.把输出框的所有信息显示到LCD屏幕中 
/*********************************
 * 
 *  参数一：LCD映射后的首地址
 *  参数二：字体框的显示位置 
 *  参数三：字体的位置 X轴
 *  参数四：字体的位置 Y轴
 *  参数五：设置好的字体输出框
 * 
 * ******************************/
void show_font_to_lcd(unsigned int *p,int px,int py,bitmap *bm);

// 关闭字体库
void fontUnload(font *f);

// 关闭bitmap
void destroyBitmap(bitmap *bm);

/******************************************
 * 
 *          函数声明
 * 
 * ****************************************/
void show_font(lcd_info_t* lcd, int x, int y, 
            char *str, int font_color, int font_size);

void show_font_number(lcd_info_t* lcd, int x, int y, 
            char *str, int length, int font_color, int font_size);

#endif