/***********************************
 *       使用各种网络协议
 *         连接服务端
 *   服务端可通过网络控制终端的广告投送
 *      以及发布紧急通知
 *  
 * * 2024-01-20
 * ********************************/

#include "network.h"

/**************************************
 * 
 *       使用tcp连接服务器
 * 
***************************************/
/************************************
 *  函数名：tcp_connect
 *  函数功能：使用tcp协议连接服务器
 *  参    数：@ip:服务器ip地址
 *           @port:服务器端口
 *  返回值：成功返回套接字，失败返回-1  
 * *********************************/
int tcp_connect(const char *ip, int port) 
{
    int sockfd;
    struct sockaddr_in servaddr;
    struct sockaddr_in self;
    int len;

    /* 创建tcp套接字 */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("tcp socket error\n");
        return -1;
    }
    /* 设置端口可复用 */
    int value = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0) 
    {
        printf("setsockopt error\n");
        return -1;
    }

    /* 修改套接字属性 */
    /* 接收超时 */
    struct timeval opt;
    opt.tv_sec = 30;
    opt.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &opt, sizeof(opt));

    /* 绑定自身的ip信息 */
    self.sin_family = AF_INET;
    self.sin_addr.s_addr = htonl(INADDR_ANY);
    self.sin_port = htons(60000);
    if (bind(sockfd, (struct sockaddr *)&self, sizeof(self)) < 0) 
    {
        printf("bind error\n");
        return -1;
    }

    /* 连接对象，服务器的信息 */
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);

    /* 连接服务器 */
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
    {
        perror("connect error:");
        return -1;
    }
    return sockfd;
}

/**************************************
 * 
 *          发送信息
 * 
 * ************************************/
int tcp_send(int sockfd, const char *buf, int len) 
{
    if(send(sockfd, buf, len, 0) < 0)
    {
        perror("send error");
        return -1;
    }
    return 0;
}

/********************************************
 * 
 *              接收信息
 * 
 * ******************************************/
int tcp_recv(int sockfd, void *buf, int len) 
{
    int ret;
    ret = recv(sockfd, buf, len, 0);
    if (ret < 0) 
    {
        perror("recv error");
        return -1;
    }
    return ret;

}

void tcp_close(int sockfd)
{
    close(sockfd);
}

/**************************
 * 
 *   使用tcp协议获取天气数据
 * 
 * ************************/
int tcp_get_weather_data(int sockfd, weather_data_t *data)
{
    if(sockfd < 0 || data == NULL)
    {
        printf("tcp_get_weather_data:invalid parameter\n");
        return -1;
    }
     /* 获取天气信息 */
    if(tcp_send(sockfd, CLIENT_GET_WEATHER, strlen(CLIENT_GET_WEATHER)) < 0)
    {
        printf("send get weather failed\n");
        return -1;
    }
    weather_data_t weather_data;
    memset(&weather_data, 0, sizeof(weather_data_t));
    if(tcp_recv(sockfd, &weather_data, sizeof(weather_data)) < 0)
    {
        printf("recv get weather failed\n");
        return -1;
    }
    strcpy(data->weather, weather_data.weather);
    data->temp = weather_data.temp;
    return 0;
}

/********************************************
 * 
 *         获取服务器的图片
 *      并显示在lcd指定的位置上
 * 
 * ******************************************/
int tcp_get_theater_data(int sockfd, int rate, int x, int y)
{
    /*********************************
     * 
     *      服务器在接收买票请求时
     *      会将剧场数据发送给客户端
     * 
     * *****************************/
    if(sockfd < 0)
    {
        printf("tcp_get_image:invalid parameter\n");
        return -1;
    }
    /* 接收剧场数据 */
    if(tcp_recv(sockfd, &theater_info, sizeof(theater_info)) < 0)
    {
        printf("recv data failed\n");
        return -1;
    }

    return 0;
}


#define MAXLINE 4096

int tcp_recv_picture(int connfd)
{
    
    FILE *fp;
    /* 清空指定文件的内容 */
    system("rm -rf /home/shao/nfs/03_project/image/temp/image.jpg");

    if((fp = fopen("/home/shao/nfs/03_project/image/temp/image.jpg","ab+") ) == NULL )
    {
        printf("File.\n");
        exit(1);
    }
    int image_size = 0;
    tcp_recv(connfd, &image_size, sizeof(image_size));
    char buf[image_size];
    memset(buf, 0, sizeof(buf));
    if(tcp_recv(connfd, buf, image_size) < 0)
    {
        printf("recv error\n");
        return -1;
    }
    fwrite(buf, 1, image_size, fp);
    fclose(fp);

    
    return 0;
}


/*****************************************
 * 
 *           udp协议通信
 * 
 * *************************************/
int udp_init(void)
{
    /* 创建套接字 */
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        printf("socket error\n");
        return -1;
    }
    // 设置套接字选项避免地址使用错误 
    int on=1;  
    if((setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    } 
    /* 绑定自身ip */
    struct sockaddr_in self; 
    memset(&self, 0, sizeof(self));
    self.sin_family = AF_INET;
    self.sin_addr.s_addr = htonl(INADDR_ANY);
    self.sin_port = htons(60010);
    if(bind(sock_fd, (struct sockaddr *)&self, sizeof(self)) < 0)
    {
        printf("bind error\n");
        return -1;
    }
    return sock_fd;
}

/***********************************
 * 
 *      使用udp接收数据
 * 
 * ********************************/
int udp_recv(int sock_fd, char *msg, int size)
{
    if(recvfrom(sock_fd, msg, size, 0, 
                NULL, NULL) < 0)
    {
        perror("sendto error");
        return -1;
    }
    return 0;
}

