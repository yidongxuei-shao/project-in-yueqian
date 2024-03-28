#ifndef MAIN_H
#define MAIN_H

/*****************************
 * 
 *      头文件
 * 
 * **************************/
#include "touch.h"
#include <stdio.h>
#include "lcd.h"
#include "read_jpeg.h"
#include "image.h"
#include "video_player.h"
#include <time.h>
#include <signal.h>
#include "data_struct.h"
#include "gui.h"
/*****************************
 * 
 *      外部声明变量
 * 
 * **************************/
extern int tcp_socket_fd;       /* tcp套接字 */
extern int udp_sock_fd;
extern weather_data_t weather_data;
extern lcd_info_t* lcd;

extern pthread_t thread_user_operation;

extern user_info_t user_info;
extern theater_info_t theater_info;
extern int is_login;

#endif // !MAIN_H

