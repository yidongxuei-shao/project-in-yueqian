#ifndef KEY_PAD_H
#define KEY_PAD_H


/* Copyright 12/8/2023 Szm */

#include "lcd.h"
#include "ts.h"
#include "jpeg.h"
#include "read_png.h"
#include "main.h"


typedef struct postion_key
{
    int x_start; 
    int x_end; 
    int y_start; 
    int y_end;

}pos_key_t;

extern pos_key_t number_1;
extern pos_key_t number_2;
extern pos_key_t number_3;
extern pos_key_t number_4;
extern pos_key_t number_5;
extern pos_key_t number_6;
extern pos_key_t number_7;
extern pos_key_t number_8;
extern pos_key_t number_9;
extern pos_key_t number_0;
extern pos_key_t number_del;
extern pos_key_t number_ture;
extern pos_key_t merch;
extern pos_key_t user;

/* 返回键 */
extern pos_key_t back;

/* 提示界面的确定与取消 */
/* 确定 */
extern pos_key_t ok;
/* 取消 */
extern pos_key_t cancel;

/* 用户界面的图标 */
extern pos_key_t user_msg;      /* 用户信息 */
extern pos_key_t charge;        /* 充电 */
extern pos_key_t recharge;      /* 充值 */



/*
    函数名称：set_postion_keypad
    函数功能：根据输入键盘的显示位置，设置每个按键的在触摸屏的坐标
             当有用户点击图片上的图案，程序对此有响应
             该输入键盘为3*4结构。
    函数参数：None
    返回值：  None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
                图案坐标是根据它在图片的位置，计算出来的
                输入键盘是在屏幕的左下角显示
        键盘图像的宽 280
                 高 310
        因此，显示的坐标位 0，480-310
*/
void set_postion_keypad(void);

/*
    函数名称：get_user_intput
    函数功能：根据用户在输入数字键盘的输入
             通过触摸坐标的范围，返回相应 0 ~ 9 的数字
             当用户按下x图案是，删除用户最后输入的一位数字
    函数参数：None
    返回值： 用户在数字键盘触摸相应的图案返回对应的0 ~ 9的数字
            按下x返回11
            按下✔时返回12
            按下返回键返回13
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
                图案坐标是根据它在图片的位置，计算出来的
                输入键盘是在屏幕的左下角显示
        键盘图像的宽 280
                 高 310
        因此，显示的坐标位 0，480-310
*/
int get_user_intput(int ts_fd);


/*
    函数名称：set_first_interface
    函数功能：根据图片上的图案，设置触摸屏捕获范围
                当有用户点击图片上的图案，程序对此有响应
    函数参数：None
    返回值：  None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
                图案坐标是根据它在图片的位置，计算出来的
        用户按钮    起始(87, 185)   
                   末尾(248, 366)

        商家按钮   起始(556, 179)   
                  末尾(723, 368)    

*/
void set_first_interface(void);

/*
    函数名称：set_true_or_false_key
    函数功能：设置提示框上的确定和取消对应图案的坐标
    函数参数：
                @x : x轴的偏移值
                @y : y轴的偏移值
    返回值： 成功返回0
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
            直接获取提示图片的宽和高，当用户点击图片的2/3的宽，高的 2/3 时定义为确定
            点击剩下的1/3, 高的 2/3 定义为取消
*/
void set_true_or_false_key(int x, int y);

/*
    函数名称：show_number
    函数功能：根据用户在输入数字键盘的输入
             在lcd上显示对应的 0 ~ 9 的数字图片
             当用户按下x图案是，删除用户最后输入的一位数字
    函数参数：None
    返回值： 成功返回0
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
                图案坐标是根据它在图片的位置，计算出来的
                输入键盘是在屏幕的左下角显示
        键盘图像的宽 280
                 高 310
        因此，显示的坐标位 0，480-310
*/
int show_number(LcdInfo_t* lcd_info, int *i, int number, char* file_path, int start_x, int start_y);

void set_user_interface_key(void);

#endif // !KEY_PAD_H#define 