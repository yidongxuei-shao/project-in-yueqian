#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "main.h"
/*****************************************
 * 
 *              服务器信息
 * 
 * **************************************/
#define SERVER_IP      "192.168.12.112"
#define SERVER_PORT     60000

/******************************************
 * 
 *              TCP请求命令
 * 
 * ****************************************/
#define CLIENT_GET_WEATHER      "get weather"
#define CLIENT_GET_TIME         "get time"   
#define CLIENT_GET_USER_INFO    "get user info" /* 获取用户信息 */
#define CLIENT_GET_CODE         "get code"      /* 请求验证码 */
#define CLIENT_REGISTER_USER    "register user" /* 注册用户信息 */
#define CLIENT_GET_TICKET       "get ticket"    /* 取票请求 */
#define CLIENT_BUY_TICKET       "buy ticket"    /* 买票请求 */
#define CLIENT_EXIT             "exit"          /* 退出 */
#define CLIENT_TURE             "true"          /* 确定 */
#define CLIENT_SEL_NEXT         "sel next"      /* 下一个 */
#define CLIENT_SEL_PREV         "sel prev"      /* 上一个 */
#define CLIENT_SYS_USER_DATA    "sys user data" /* 同步用户信息 */
/* 座位数据请求 */
#define CLIENT_GET_SEAT_DATA    "get seat data"

/******************************************
 * 
 *              函数声明
 * 
 * ****************************************/
int tcp_connect(const char *ip, int port);
int tcp_send(int sockfd, const char *buf, int len);
int tcp_recv(int sockfd, void *buf, int len);
void tcp_close(int sockfd);

int tcp_get_weather_data(int sockfd, weather_data_t *data);
int tcp_send_user_data(int sockfd, char *account, char *password);

int tcp_get_theater_data(int sockfd, int rate, int x, int y);

#if 0
int tcp_get_image_data(int sockfd, int rate, int x, int y);
int tcp_recv_picture(int conned);
// int tcp_recv_picture2(int client_socket, const char *file_name);
#endif

int udp_init(void);
int udp_recv(int sock_fd, char *msg, int size);
/*****************************************/

#endif // !NETWORK