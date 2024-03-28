/*******************************************
 * 
 *          服务器端网络连接
 * 
 * *****************************************/

#ifndef NETWORK_H
#define NETWORK_H

/**************************
 * 
 *          头文件 
 * 
 * ***********************/
/********************************
 * 
 *      系统头文件
 * 
 * ******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#include "cJSON.h"
#include "main.h"
/******************************
 * 
 *        天气域名
 * 
 * ***************************/
#define WEATHER_DOMAIN "weatherapi.com"
#define HTTP_PORT 80

/*****************************
 * 
 *      局域网连接端口
 * 
 * ***************************/
#define LOCAL_PORT 60000

#define UDP_PORT    60010

/*****************************
 * 
 *      客户端的资源请求
 * 
 * ***************************/
#define CLIENT_GET_WEATHER      "get weather"
#define CLIENT_GET_TIME         "get time"   
#define CLINET_GET_USER_INFO    "get user info" /* 获取用户信息 */
#define CLINET_GET_CODE         "get code"      /* 请求验证码 */
#define CLINET_REGISTER_USER    "register user" /* 注册用户信息 */
#define CLIENT_GET_TICKET       "get ticket"    /* 取票请求 */
#define CLIENT_BUY_TICKET       "buy ticket"    /* 买票请求 */
#define CLIENT_EXIT             "exit"          /* 退出 */
#define CLIENT_TURE             "true"          /* 确定 */
#define CLIENT_SEL_NEXT         "sel next"      /* 下一个 */
#define CLIENT_SEL_PREV         "sel prev"      /* 上一个 */
#define CLIENT_SYS_USER_DATA    "sys user data" /* 同步用户信息 */
/* 座位数据请求 */
#define CLIENT_GET_SEAT_DATA    "get seat data"
/*****************************
 * 
 *          地区
 * 
 * ***************************/
#define Guangzhou "Guangzhou"
#define Shanghai "Shanghai"
#define Chongqing "Chongqing"

/******************************
 * 
 *      结构体变量
 *      用于存储天气数据
 * ***************************/
typedef struct weather_data{
    int temp;
    char weather[40];
}weather_data_t;
/****************************
 * 
 *          函数声明 
 * 
 * **************************/
int listen_network(int sock_fd ,int port, struct sockaddr_in* dest);
void* server_start(void* arg);

theater_info_t* tcp_theater_send_data(theater_head_node_t *Pmanager, 
                int accept_sock_fd);
int tcp_send_pictrue(int sockfd, theater_info_t *data);

int udp_init(void);
int udp_send(int sock_fd, char *ip, int port, char *msg);
#endif // !NET