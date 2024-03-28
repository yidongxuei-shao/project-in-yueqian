#ifndef GUI_H
#define GUI_H

/******************************
 * 
 *          头文件
 *  
 * ***************************/
#include "lcd.h"
#include "font.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "other.h"
#include "image.h"
#include "read_png.h"
#include "read_jpeg.h"
#include "my_thread.h"
#include "network.h"
/*******************************
 * 
 *          函数声明
 * 
 * ****************************/
void show_time(lcd_info_t* lcd, int x, int y);
int show_weather(lcd_info_t* lcd, weather_data_t* weather ,int x, int y);
int show_user_operation(lcd_info_t* lcd);

int show_login(lcd_info_t* lcd);
int show_number_lcd(lcd_info_t* lcd, int x, int y, char* output_data, int data_len);
int show_register_lcd(lcd_info_t* lcd);

int show_user_center(lcd_info_t* lcd);
int show_user_center_tickinfo(int num);

void show_theater_info(theater_info_t *theater_info);
time_table_t * theater_time_table_init(theater_info_t * theater_info, int* array_len);
int show_time_table(time_table_t *time_table, int count, int array_len);

int show_select_ticket(lcd_info_t* lcd, theater_info_t *theater_info, int tcp_socket);

int write_theater_data_in_user(theater_info_t* theater_info, user_info_t *user_info);

int get_seat_number(int *seat_x, int* seat_y, int *bought_buf, int array_len);
int show_select_seat(lcd_info_t *lcd, 
                int accept_socket_fd, theater_info_t* theater_info);
int show_pay(lcd_info_t *lcd, int accept_socket_fd, theater_info_t* theater_info, 
            user_info_t* user_info);

/*********************************
 * 
 *          返回值
 * 
 * *******************************/
#define BREAK_LAST     11        /* 返回上一层 */
#define GOTO_NEXT      12        /* 下一步 */


/******************************
 * 
 *     主界面功能按键的坐标
 * 
 * ***************************/
#define BUTTON_MAIN_WIDTH       180
#define BUTTON_MAIN_HEIGHT      50         

#define BUTTON_MAIN_GET_X       566
#define BUTTON_MAIN_GET_Y       115

#define BUTTON_MAIN_BUY_X       566
#define BUTTON_MAIN_BUY_Y       226

#define BUTTON_MAIN_SHOPPING_X      566
#define BUTTON_MAIN_SHOPPING_Y      338


/******************************
 * 
 *      登录界面按键坐标
 * 
 * ****************************/
/* 按键长度 */
#define BUTTON_LOGIN_WIDTH       148
#define BUTTON_LOGIN_HEIGHT      50
/* 用户登录 */
#define BUTTON_LOGIN_USER_X       465  
#define BUTTON_LOGIN_USER_Y       316
/* 用户注册 */
#define BUTTON_LOGIN_REGISTER_X       625
#define BUTTON_LOGIN_REGISTER_Y       316

/* 管理员按键坐标 */
#define BUTTON_LOGIN_ADMIN_WIDTH       140
#define BUTTON_LOGIN_ADMIN_HEIGHT      40
#define BUTTON_LOGIN_ADMIN_X    620
#define BUTTON_LOGIN_ADMIN_Y    420

/* 登录可视按键 */
#define BUTTON_LOGIN_VISIBLE_X           772
#define BUTTON_LOGIN_VISIBLE_Y           234

/* 账号输入框 */
#define BUTTON_LOGIN_USER_WIDTH         307
#define BUTTON_LOGIN_USER_HEIGHT        44

#define BUTTON_LOGIN_ACCOUNT_INPUT_X       465
#define BUTTON_LOGIN_ACCOUNT_INPUT_Y       119
/* 密码输入框 */
#define BUTTON_LOGIN_PASSWORD_INPUT_X       465
#define BUTTON_LOGIN_PASSWORD_INPUT_Y       227


/**************************************
 * 
 *          注册按键坐标
 * 
 * ***********************************/
#define BUTTON_REGISTER_WIDTH       140     /* 注册按键长度 */
#define BUTTON_REGISTER_HEIGHT      60
#define BUTTON_REGISTER_X           556
#define BUTTON_REGISTER_Y           384

/* 发送按键 */
#define BUTTON_REGISTER_SEND_WIDTH       80
#define BUTTON_REGISTER_SEND_HEIGHT      35
#define BUTTON_REGISTER_SEND_X           700
#define BUTTON_REGISTER_SEND_Y           310

/* 可视按键 */
#define BUTTON_VISIBLE_WIDTH       40
#define BUTTON_VISIBLE_HEIGHT      30
#define BUTTON_REGISTER_VISIBLE_X           739
#define BUTTON_REGISTER_VISIBLE_Y           222

/* 输入框的宽和高 */
#define REGISTER_INPUT_WIDTH         315
#define REGISTER_INPUT_HEIGHT        44

/* 电话号码输入框 */
#define REGISTER_PHONE_INPUT_X       468
#define REGISTER_PHONE_INPUT_Y       123
/* 密码输入框 */
#define REGISTER_PASSWORD_INPUT_X       468
#define REGISTER_PASSWORD_INPUT_Y       215

/* 验证码的宽和高 */
#define REGISTER_CODE_INPUT_WIDTH         200
#define REGISTER_CODE_INPUT_HEIGHT        44

/* 验证码输入框 */
#define REGISTER_CODE_INPUT_X       468
#define REGISTER_CODE_INPUT_Y       304


/* 返回按键坐标 */
#define BUTTON_BACK_WIDTH       60
#define BUTTON_BACK_HEIGHT      60
#define BUTTON_BACK_X       5
#define BUTTON_BACK_Y       5


/******************************
 * 
 *      数字键盘坐标
 * 
 * ****************************/
#define KEYBOARD_WIDTH       112
#define KEYBOARD_HEIGHT      50
/* 间隔 9像素 */
#define KEY_WIDTH       9       /* 宽 */
#define KEY_HEIGHT      11      /* 高 */

/* 数字1 */
#define KEYBOARD_1_X       49
#define KEYBOARD_1_Y       187
/* 数字2 */
#define KEYBOARD_2_X       (KEYBOARD_1_X + KEY_WIDTH + KEYBOARD_WIDTH)
#define KEYBOARD_2_Y       KEYBOARD_1_Y
/* 数字3 */
#define KEYBOARD_3_X       (KEYBOARD_2_X + KEY_WIDTH + KEYBOARD_WIDTH) 
#define KEYBOARD_3_Y       KEYBOARD_1_Y 

/* 数字4 */
#define KEYBOARD_4_X       KEYBOARD_1_X
#define KEYBOARD_4_Y       (KEYBOARD_1_Y + KEY_HEIGHT + KEYBOARD_HEIGHT)
/* 数字5 */
#define KEYBOARD_5_X       (KEYBOARD_4_X + KEY_WIDTH + KEYBOARD_WIDTH)
#define KEYBOARD_5_Y       KEYBOARD_4_Y
/* 数字6 */
#define KEYBOARD_6_X       (KEYBOARD_5_X + KEY_WIDTH + KEYBOARD_WIDTH)
#define KEYBOARD_6_Y       KEYBOARD_4_Y

/* 数字7 */
#define KEYBOARD_7_X       KEYBOARD_4_X
#define KEYBOARD_7_Y       (KEYBOARD_4_Y + KEY_HEIGHT + KEYBOARD_HEIGHT)
/* 数字8 */
#define KEYBOARD_8_X       (KEYBOARD_7_X + KEY_WIDTH + KEYBOARD_WIDTH)
#define KEYBOARD_8_Y       KEYBOARD_7_Y
/* 数字9 */
#define KEYBOARD_9_X       (KEYBOARD_8_X + KEY_WIDTH + KEYBOARD_WIDTH)
#define KEYBOARD_9_Y       KEYBOARD_7_Y
/* 数字0 */
#define KEYBOARD_0_X       105
#define KEYBOARD_0_Y       370
/* 删除 */
#define KEYBOARD_DEL_X       234
#define KEYBOARD_DEL_Y       370

/*******************************************
 * 
 *            用户中心界面功能按键坐标
 * 
 * *****************************************/
#define BUTTON_USER_CENTER_WIDTH       138
#define BUTTON_USER_CENTER_HEIGHT      65         

/* 取票 */
#define BUTTON_USER_CENTER_GET_X       390
#define BUTTON_USER_CENTER_GET_Y       395
/* 买票 */
#define BUTTON_USER_CENTER_BUY_X       616
#define BUTTON_USER_CENTER_BUY_Y       395
/* 密码设置 */
#define BUTTON_USER_CENTER_SET_WIDTH      207
#define BUTTON_USER_CENTER_SET_HEIGHT     65
#define BUTTON_USER_CENTER_SETTING_X      86
#define BUTTON_USER_CENTER_SETTING_Y      395

/* 下一张 */
#define BUTTON_USER_CENTER_NEXT_WIDTH       70
#define BUTTON_USER_CENTER_NEXT_HEIGHT      70
#define BUTTON_USER_CENTER_NEXT_X       720
#define BUTTON_USER_CENTER_NEXT_Y       230

/* 上一张 */
#define BUTTON_USER_CENTER_PRE_WIDTH       70
#define BUTTON_USER_CENTER_PRE_HEIGHT      70
#define BUTTON_USER_CENTER_PRE_X       10
#define BUTTON_USER_CENTER_PRE_Y       230

/*******************
 * 
 *    输出框
 * 
 * ****************/
/* 间隔 8个像素 */
#define OUTPUT_CENTER_HEIGHT            8
#define OUTPUT_CENTER_WINDOW_HEIGHT             28
/* 影片 */
#define USER_CENTER_OUTPUT_MOVE_X        (235 + 10)
#define USER_CENTER_OUTPUT_MOVE_Y        178
/* 价格 */
#define USER_CENTER_OUTPUT_PRICE_X        (235 + 10)
#define USER_CENTER_OUTPUT_PRICE_Y        (USER_CENTER_OUTPUT_MOVE_Y +  OUTPUT_CENTER_HEIGHT + OUTPUT_CENTER_WINDOW_HEIGHT)
/* 开始时间 */
#define USER_CENTER_OUTPUT_START_X       (235 + 10)
#define USER_CENTER_OUTPUT_START_Y        (USER_CENTER_OUTPUT_PRICE_Y + OUTPUT_CENTER_HEIGHT + OUTPUT_CENTER_WINDOW_HEIGHT)
/* 剧场编号 */
#define USER_CENTER_OUTPUT_ROOM_X        (235 + 10)
#define USER_CENTER_OUTPUT_ROOM_Y        (USER_CENTER_OUTPUT_START_Y + OUTPUT_CENTER_HEIGHT + OUTPUT_CENTER_WINDOW_HEIGHT)
/* 座位号 */
#define USER_CENTER_OUTPUT_SEAT_X       (235 + 10)
#define USER_CENTER_OUTPUT_SEAT_Y        (USER_CENTER_OUTPUT_ROOM_Y + OUTPUT_CENTER_HEIGHT + OUTPUT_CENTER_WINDOW_HEIGHT)
/* 账号余额 */
#define USER_CENTER_OUTPUT_BALANCE_X       (276 + 10)
#define USER_CENTER_OUTPUT_BALANCE_Y     (102 + 10)
/* 当前票数 */
#define USER_CENTER_OUTPUT_NUMBER_X       (631 + 10)
#define USER_CENTER_OUTPUT_NUMBER_Y     (103 + 10)

/**********************************
 * 
 *      选票输出框
 * 
 * ********************************/
/* 间隔 12个像素 */
#define SELECT_OUTPUT_PEIR_HEIGHT            12
#define SELECT_OUTPUT_WIDTH       300
#define SELECT_OUTPUT_HEIGHT      32

#define SELECT_OUTPUT_MOVE_X           (272 + 20)
#define SELECT_OUTPUT_MOVE_Y           265

#define SELECT_OUTPUT_PRICE_X           SELECT_OUTPUT_MOVE_X
#define SELECT_OUTPUT_PRICE_Y           (SELECT_OUTPUT_MOVE_Y + SELECT_OUTPUT_HEIGHT + SELECT_OUTPUT_PEIR_HEIGHT)

#define SELECT_OUTPUT_NUMBER_X           SELECT_OUTPUT_PRICE_X
#define SELECT_OUTPUT_NUMBER_Y          (SELECT_OUTPUT_PRICE_Y + SELECT_OUTPUT_HEIGHT + SELECT_OUTPUT_PEIR_HEIGHT)

/* 按键宽和高 */
#define BUTTON_SELECT_WIDTH       144
#define BUTTON_SELECT_HEIGHT      51
/* 确定按键 */
#define BUTTON_SELECT_CONFIRM_X       626
#define BUTTON_SELECT_CONFIRM_Y       412

/* 输出的图片 */
#define SELECT_OUTPUT_IMAGE_X       300
#define SELECT_OUTPUT_IMAGE_Y       25

/************************************
 * 
 *          时间选择界面
 * 
 * **********************************/
#define TIME_SELECT_WIDTH       450
#define TIME_SELECT_HEIGHT      50
/* 间隔 */
#define TIME_SELECT_PEIR_HEIGHT            23
/* 时间一的输出框 */
#define TIME_SELECT_ONE_X           (170 + 50)
#define TIME_SELECT_ONE_Y           (110 + 10)
/* 时间二的输出框 */
#define TIME_SELECT_TWO_X           TIME_SELECT_ONE_X
#define TIME_SELECT_TWO_Y           (TIME_SELECT_ONE_Y + TIME_SELECT_HEIGHT + TIME_SELECT_PEIR_HEIGHT)
/* 时间三的输出框 */
#define TIME_SELECT_THREE_X         TIME_SELECT_TWO_X
#define TIME_SELECT_THREE_Y         (TIME_SELECT_TWO_Y + TIME_SELECT_HEIGHT + TIME_SELECT_PEIR_HEIGHT)
/* 时间四的输出框 */
#define TIME_SELECT_FOUR_X          TIME_SELECT_THREE_X
#define TIME_SELECT_FOUR_Y          (TIME_SELECT_THREE_Y + TIME_SELECT_HEIGHT + TIME_SELECT_PEIR_HEIGHT)

/*******************************************
 * 
 *              座位表
 * 
 * ****************************************/
#define SEAT_SELECT_WIDTH       50
#define SEAT_SELECT_HEIGHT      48
/* 间距宽 */
#define SEAT_SELECT_PEIR_WIDTH            25
/* 间距高 */
#define SEAT_SELECT_PEIR_HEIGHT           25

/* 座位1 */
#define SEAT_SELECT_1_X           112
#define SEAT_SELECT_1_Y           120
/* 座位2 */
#define SEAT_SELECT_2_X           (SEAT_SELECT_1_X + SEAT_SELECT_WIDTH + SEAT_SELECT_PEIR_WIDTH)
#define SEAT_SELECT_2_Y           SEAT_SELECT_1_Y
/* 座位3 */
#define SEAT_SELECT_3_X           (SEAT_SELECT_2_X + SEAT_SELECT_WIDTH + SEAT_SELECT_PEIR_WIDTH)
#define SEAT_SELECT_3_Y           SEAT_SELECT_2_Y
/* 座位4 */
#define SEAT_SELECT_4_X           (SEAT_SELECT_3_X + SEAT_SELECT_WIDTH + SEAT_SELECT_PEIR_WIDTH)
#define SEAT_SELECT_4_Y           SEAT_SELECT_3_Y
/* 座位5 */
#define SEAT_SELECT_5_X           (SEAT_SELECT_4_X + SEAT_SELECT_WIDTH + SEAT_SELECT_PEIR_WIDTH)
#define SEAT_SELECT_5_Y           SEAT_SELECT_4_Y
/* 座位6 */
#define SEAT_SELECT_6_X           (SEAT_SELECT_5_X + SEAT_SELECT_WIDTH + SEAT_SELECT_PEIR_WIDTH)
#define SEAT_SELECT_6_Y           SEAT_SELECT_5_Y
/* 座位7 */
#define SEAT_SELECT_7_X           (SEAT_SELECT_6_X + SEAT_SELECT_WIDTH + SEAT_SELECT_PEIR_WIDTH)
#define SEAT_SELECT_7_Y           SEAT_SELECT_6_Y
/* 座位8 */
#define SEAT_SELECT_8_X           (SEAT_SELECT_7_X + SEAT_SELECT_WIDTH + SEAT_SELECT_PEIR_WIDTH)
#define SEAT_SELECT_8_Y           SEAT_SELECT_7_Y

/* 座位9 */
#define SEAT_SELECT_9_X           SEAT_SELECT_1_X
#define SEAT_SELECT_9_Y           (SEAT_SELECT_1_Y + SEAT_SELECT_PEIR_HEIGHT + SEAT_SELECT_HEIGHT)
/* 座位10 */
#define SEAT_SELECT_10_X           SEAT_SELECT_2_X
#define SEAT_SELECT_10_Y           SEAT_SELECT_9_Y
/* 座位11 */
#define SEAT_SELECT_11_X           SEAT_SELECT_3_X
#define SEAT_SELECT_11_Y           SEAT_SELECT_9_Y
/* 座位12 */
#define SEAT_SELECT_12_X           SEAT_SELECT_4_X
#define SEAT_SELECT_12_Y           SEAT_SELECT_9_Y
/*  座位13 */
#define SEAT_SELECT_13_X           SEAT_SELECT_5_X
#define SEAT_SELECT_13_Y           SEAT_SELECT_9_Y
/* 座位14 */
#define SEAT_SELECT_14_X           SEAT_SELECT_6_X
#define SEAT_SELECT_14_Y           SEAT_SELECT_9_Y
/* 座位15 */
#define SEAT_SELECT_15_X           SEAT_SELECT_7_X
#define SEAT_SELECT_15_Y           SEAT_SELECT_9_Y
/* 座位16 */
#define SEAT_SELECT_16_X           SEAT_SELECT_8_X
#define SEAT_SELECT_16_Y           SEAT_SELECT_9_Y

/* 座位17 */
#define SEAT_SELECT_17_X           SEAT_SELECT_1_X
#define SEAT_SELECT_17_Y           (SEAT_SELECT_9_Y + SEAT_SELECT_PEIR_HEIGHT + SEAT_SELECT_HEIGHT)
/* 座位18 */
#define SEAT_SELECT_18_X           SEAT_SELECT_2_X
#define SEAT_SELECT_18_Y           SEAT_SELECT_17_Y
/* 座位19 */
#define SEAT_SELECT_19_X           SEAT_SELECT_3_X
#define SEAT_SELECT_19_Y           SEAT_SELECT_17_Y
/* 座位20 */
#define SEAT_SELECT_20_X           SEAT_SELECT_4_X
#define SEAT_SELECT_20_Y           SEAT_SELECT_17_Y
/* 座位21 */
#define SEAT_SELECT_21_X           SEAT_SELECT_5_X
#define SEAT_SELECT_21_Y           SEAT_SELECT_17_Y
/* 座位22 */
#define SEAT_SELECT_22_X           SEAT_SELECT_6_X
#define SEAT_SELECT_22_Y           SEAT_SELECT_17_Y
/* 座位23 */
#define SEAT_SELECT_23_X           SEAT_SELECT_7_X
#define SEAT_SELECT_23_Y           SEAT_SELECT_17_Y
/* 座位24 */
#define SEAT_SELECT_24_X           SEAT_SELECT_8_X
#define SEAT_SELECT_24_Y           SEAT_SELECT_17_Y

/* 座位25 */
#define SEAT_SELECT_25_X           SEAT_SELECT_1_X
#define SEAT_SELECT_25_Y           (SEAT_SELECT_17_Y + SEAT_SELECT_PEIR_HEIGHT + SEAT_SELECT_HEIGHT)
/* 座位26 */
#define SEAT_SELECT_26_X           SEAT_SELECT_2_X
#define SEAT_SELECT_26_Y           SEAT_SELECT_25_Y
/* 座位27 */
#define SEAT_SELECT_27_X           SEAT_SELECT_3_X
#define SEAT_SELECT_27_Y           SEAT_SELECT_25_Y
/* 座位28 */
#define SEAT_SELECT_28_X           SEAT_SELECT_4_X
#define SEAT_SELECT_28_Y           SEAT_SELECT_25_Y
/* 座位29 */
#define SEAT_SELECT_29_X           SEAT_SELECT_5_X
#define SEAT_SELECT_29_Y           SEAT_SELECT_25_Y
/* 座位30 */
#define SEAT_SELECT_30_X           SEAT_SELECT_6_X
#define SEAT_SELECT_30_Y           SEAT_SELECT_25_Y
/* 座位31 */
#define SEAT_SELECT_31_X           SEAT_SELECT_7_X
#define SEAT_SELECT_31_Y           SEAT_SELECT_25_Y
/* 座位32 */
#define SEAT_SELECT_32_X           SEAT_SELECT_8_X
#define SEAT_SELECT_32_Y           SEAT_SELECT_25_Y

/* 确定按键 */
#define SEAT_SELECT_CONFIRM_X     630
#define SEAT_SELECT_CONFIRM_Y     420
#define SEAT_SELECT_CONFIRM_WIDTH  120
#define SEAT_SELECT_CONFIRM_HEIGHT  50

/********************************
 * 
 *          付款界面
 * 
 * ******************************/
#define PAYMENT_OUTPUT_WIDTH       390
#define PAYMENT_OUTPUT_HEIGHT      40
/* 间距 */
#define PAYMENT_OUTPUT_PEIR_WIDTH  21
/* 影片数据框 */
#define PAYMENT_OUTPUT_MOVIE_X     (220 + 50)
#define PAYMENT_OUTPUT_MOVIE_Y     (120 + 10)
/* 剧场编号 */
#define PAYMENT_OUTPUT_THEATER_NO_X     PAYMENT_OUTPUT_MOVIE_X
#define PAYMENT_OUTPUT_THEATER_NO_Y     (PAYMENT_OUTPUT_MOVIE_Y + PAYMENT_OUTPUT_HEIGHT + PAYMENT_OUTPUT_PEIR_WIDTH)
/* 开始时间 */
#define PAYMENT_OUTPUT_START_TIME_X     PAYMENT_OUTPUT_MOVIE_X
#define PAYMENT_OUTPUT_START_TIME_Y     (PAYMENT_OUTPUT_THEATER_NO_Y + PAYMENT_OUTPUT_HEIGHT + PAYMENT_OUTPUT_PEIR_WIDTH)
/* 座位号 */
#define PAYMENT_OUTPUT_SEAT_X     PAYMENT_OUTPUT_MOVIE_X
#define PAYMENT_OUTPUT_SEAT_Y     (PAYMENT_OUTPUT_START_TIME_Y + PAYMENT_OUTPUT_HEIGHT + PAYMENT_OUTPUT_PEIR_WIDTH)
/* 价格 */
#define PAYMENT_OUTPUT_PRICE_X     PAYMENT_OUTPUT_MOVIE_X
#define PAYMENT_OUTPUT_PRICE_Y     (PAYMENT_OUTPUT_SEAT_Y + PAYMENT_OUTPUT_HEIGHT + PAYMENT_OUTPUT_PEIR_WIDTH)

/* 确定按键 */
#define PAYMENT_CONFIRM_X     620
#define PAYMENT_CONFIRM_Y     416
#define PAYMENT_CONFIRM_WIDTH  135
#define PAYMENT_CONFIRM_HEIGHT  47

/*******************
 * 
 *      支付界面的倒计时位置
 * 
 * *****************/
#define PAYMENT_COUNTDOWN_X     (656 + 40)
#define PAYMENT_COUNTDOWN_Y     (350 + 10)

/************************
 * 
 *      施工界面的倒计时位置
 * 
 * **********************/
#define UNDO_COUNTDOWN_X     356
#define UNDO_COUNTDOWN_Y     405

#endif 