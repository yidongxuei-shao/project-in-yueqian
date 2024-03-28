#ifndef MY_THREAD_H
#define MY_THREAD_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "touch.h"
#include "lcd.h"
#include "read_jpeg.h"
#include "image.h"
#include "video_player.h"
#include <signal.h>
#include "gui.h"
#include "dir_scan.h"
/**************************************
 * 
 *          任务函数声明
 * 
 * ***********************************/
void* thread_start_animation(void* arg);
void* touch_thread(void* arg);
void* thread_start_video(void* arg);
void* task_user_key_main(void* arg);

/**************************************
 * 
 *            临界资源
 * 
 * ***********************************/
extern volatile int touch_x, touch_y;
extern volatile int touch_flag;

extern int touch_ret_val;
extern pthread_mutex_t touch_ret_val_mutex;

extern int user_ret_val;       /* 用户区分用户的在main界面中按下的按键 */
/**************************************
 * 
 *          宏定义
 * 
 * ************************************/
#define GET_TICKS       4   /* 取票 */
#define BUY_TICKS       5   /* 买票 */
#define SHOPPING        6   /* 商城 */

/* 用户账号长度 */
#define USER_ACCOUNT_LEN    11      /* 固定11个字符 */
#define USER_PASSWORD_LEN    6     /* 固定6个字符 */

/* 验证码长度 */
#define USER_CODE_LEN    4
#endif // !MY_THREAD_H