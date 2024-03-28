#include "font.h"
#include <stdio.h>
#include "lcd.h"
#include <string.h>

#define FONT_BACKGROUND_COLOR getColor(0,255,255,255)     /* 字体背景色 */
#define FONT_LEN        200
void show_font(lcd_info_t* lcd, int x, int y, 
            char *str, int font_color, int font_size)
{
	//打开字体	
	font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");

	//字体大小的设置
	fontSetSize(f, font_size);
	//创建一个画板（点阵图）
	bitmap *bm = createBitmapWithInit(FONT_LEN, 
                    font_size, lcd->colour_deep, FONT_BACKGROUND_COLOR);
	
	//将字体写到点阵图上
	fontPrint(f, bm, 0, 0, str, font_color, 0);
  	//把字体框输出到LCD屏幕上
	show_font_to_lcd(lcd->lcd_mp, x, y, bm);

	//关闭字体，关闭画板
	fontUnload(f);
	destroyBitmap(bm);
}

void show_font_number(lcd_info_t* lcd, int x, int y, 
            char *str, int length, int font_color, int font_size)
{
	//打开字体	
	font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");

	//字体大小的设置
	fontSetSize(f, font_size);
	//创建一个画板（点阵图）
	bitmap *bm = createBitmapWithInit(length, 
                    font_size, lcd->colour_deep, FONT_BACKGROUND_COLOR);
	
	//将字体写到点阵图上
	fontPrint(f, bm, 0, 0, str, font_color, 0);
  	//把字体框输出到LCD屏幕上
	show_font_to_lcd(lcd->lcd_mp, x, y, bm);

	//关闭字体，关闭画板
	fontUnload(f);
	destroyBitmap(bm);
}
