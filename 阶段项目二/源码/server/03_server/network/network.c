/*******************************************
 * 
 *          服务器端网络连接
 * 
 * *****************************************/
#include "network.h"

int tcp_sock_fd = 0;
int udp_sock_fd = 0;

/*
    测试网络连接
*/
int connect_network(char *ip, char* domain, int port)
{
    int sock_fd;
    struct sockaddr_in addr;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        printf("socket error\n");
        return -1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if(ip == NULL && domain != NULL)
    {
        /* 解析域名 */
        struct  hostent *dest = gethostbyname("api.seniverse.com");
        if(dest == NULL)
        {
            perror("gethostbyname error");
            return -1;
        }
        char buf[30] = {0};   
        /* 解析后的IP地址 */
        inet_ntop(dest->h_addrtype,dest->h_addr_list[0],
                                    buf, sizeof(buf));
        addr.sin_addr.s_addr = inet_addr(buf);
    }
    else
    {
         addr.sin_addr.s_addr = inet_addr(ip);
    }
   
    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("connect error\n");
        return -1;
    }

    return sock_fd;
}

/******************************************
 * 
 *         向心智天气发送请求
 *          返回天气情况和温度
 * ****************************************/
int get_weather(char *where, weather_data_t* data)
{
   int tcp_sock_fd = connect_network(NULL, WEATHER_DOMAIN, HTTP_PORT);
    if(tcp_sock_fd < 0)
    {
        printf("tcp_sock_fd error\n");
        return -1;
    }
    printf("tcp_sock_fd success\n");

    /* 设置超时时间 */
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    setsockopt(tcp_sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    /* 发送消息 */
    /* 发送数据 */
    char http_hand[150] = {0};
    /* 方法 + 空格 + 资源路径（URL）+ 空格 + HTTP版本(HTTP/1.1) + \r\n + \n +\r\n */
    sprintf(http_hand,
    "GET https://api.seniverse.com/v3/weather/now.json?key=Sv-l0T_2UIFgO8xIr&location=%s&language=en&unit=c HTTP/1.1\r\nHost: api.seniverse.com\r\n\n\r\n", where);

    char recv_buf[1280] = {0};
    if(send(tcp_sock_fd, http_hand, strlen(http_hand), 0) < 0)
    {
        perror("send error");
        return -1;
    }

    /* 接收数据 */
    if(recv(tcp_sock_fd, recv_buf, sizeof(recv_buf), 0) < 0)
    {
        perror("recv error");
        return -1;
    }
    // printf("%s", recv_buf);
    bzero(recv_buf, sizeof(recv_buf));

    if(recv(tcp_sock_fd, recv_buf, sizeof(recv_buf), 0) < 0)
    {
        perror("recv error");
        return -1;
    }
    // printf("%s\r\n", recv_buf);
    /* Json格式解析 */
     /* 解析json数据 */
    cJSON *root = cJSON_Parse(recv_buf); /* 将接收的字符串转化为json格式 */
    
    /* 从顶层对象中获取数据 */
    cJSON* results = cJSON_GetObjectItem(root, "results");

    /* 从数组中获取数据 */
    cJSON* obj_1 = cJSON_GetArrayItem(results, 0);

    /* 从obj_1中获取数据，也就是 "now"" */
    cJSON* obj_2_2 = cJSON_GetObjectItem(obj_1, "now");

    /* 从obj_2_2中获取数据，也就是 "temperature" */
    cJSON* temperature = cJSON_GetObjectItem(obj_2_2, "temperature");
    /* 从obj_2_2中获取数据，也就是 "text" */
    cJSON* text = cJSON_GetObjectItem(obj_2_2, "text");

    /* 打印json数据 */
    // printf("%s\r\n", cJSON_Print(temperature));
    // printf("当前的温度为 [%d] \r\n", atoi(temperature->valuestring));
    /* 赋值 */
    memset(data, 0, sizeof(weather_data_t));
    data->temp = atoi(temperature->valuestring);
    strcpy(data->weather, text->valuestring);
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
    /* 开启广播 */
    int broadcast = 1;
    if((setsockopt(sock_fd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }
    return sock_fd;
}

/***********************************
 * 
 *      使用udp发送数据
 * 
 * ********************************/
int udp_send(int sock_fd, char *ip, int port, char *msg)
{
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(ip);

    if(sendto(sock_fd, msg, strlen(msg), 0, 
                (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
    {
        perror("sendto error");
        return -1;
    }
    return 0;
}


/*****************************************
 * 
 *              监听网络
 * 
 * ***************************************/
int tcp_init(int port)
{
    struct sockaddr_in self;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        printf("socket error\n");
        return -1;
    }
    /* 设置端口可复用 */
    int value = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0) 
    {
        printf("setsockopt error\n");
        return -1;
    }
    /* 绑定自身ip */
    self.sin_family = AF_INET;
    self.sin_port = htons(port);
    self.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock_fd, (struct sockaddr *)&self, sizeof(self)))
    {
        perror("can not bind self");
        return -1;
    }
    printf("bind success\n");
    /* 返回能正常通讯的套接字 */
    return sock_fd;
}
int listen_network(int sock_fd ,int port, struct sockaddr_in* dest)
{
    /* 监听端口 */
    listen(sock_fd, 5);

    /* 使用udp发送信息 */
    udp_send(udp_sock_fd, "192.168.12.255", UDP_PORT, CLIENT_TURE);

    printf("listening...\r\n");
    /* 接收客户端连接 */
    socklen_t len = 0;
    int accept_sock_fd = accept(sock_fd, (struct sockaddr *)dest, &len);
    if(accept_sock_fd < 0)
    {
        perror("accept error");
        return -1;
    }
    /* 设置接收超时 */
    // struct timeval tv;
    // tv.tv_sec = 10;
    // tv.tv_usec = 0;
    // setsockopt(accept_sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
   

    return accept_sock_fd;
}
/******************************************
 * 
 *            作为服务器开始运行
 * 
 * ******************************************/

void* server_start(void* arg)
{
    /* 创建ucp套接字 */
    udp_sock_fd = udp_init();
    if (udp_sock_fd < 0)
    {
        printf("udp_init error\n");
        return NULL;
    }
    tcp_sock_fd = tcp_init(LOCAL_PORT);
    // struct sockaddr_in client_addr;
    // tcp_sock_fd = listen_network(LOCAL_PORT, &client_addr);
    if(tcp_sock_fd < 0)
    {
        printf("can not accept client\r\n");
        return NULL;
    }

    user_info_t user_data;
    memset(&user_data, 0, sizeof(user_info_t));
    /* 接收来自客户端的请求 */
    char recv_buf[50] = {0};
    while(1)
    {
        struct sockaddr_in client_addr;
        int accept_sock_fd = listen_network(tcp_sock_fd ,LOCAL_PORT, &client_addr);
        if(accept_sock_fd < 0)
        {
            printf("can not accept client\r\n");
            break;
        }
        theater_info_t* theater_temp = NULL;
        while(1)
        {
           
            memset(recv_buf, 0, sizeof(recv_buf));
           
            /* 接收客户端请求 */
            if(recv(accept_sock_fd, recv_buf, sizeof(recv_buf), 0) < 0)
            {
                perror("recv error");
                break;
            }
            /* 分析来自客户端的请求 */
            if(strcmp(recv_buf, CLIENT_GET_TIME) == 0)
            {
                time_t t = time(NULL);
                struct tm* time = localtime(&t);
                char time_buf[128] = {0};
                sprintf(time_buf, "%d-%d-%d/%d:%d:%d", 
                    time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, 
                    time->tm_hour, time->tm_min, time->tm_sec);
                send(accept_sock_fd, (void*)time_buf, strlen(time_buf), 0);
            }
            else if(strcmp(recv_buf, CLIENT_GET_WEATHER) == 0)
            {
                weather_data_t data;
                get_weather(Guangzhou, &data);
                printf("temp = %d, text = %s\r\n", data.temp, data.weather);
                send(accept_sock_fd, (void *)&data, sizeof(data), 0);
            }
            else if(strcmp(recv_buf, CLINET_GET_CODE) == 0) /* 请求验证码 */
            {
                char buf[30] = {0};
                if(recv(accept_sock_fd, buf, sizeof(buf), 0) < 0)
                {
                    perror("recv account error");
                    return NULL;
                }
                /* 数据处理 */
                printf("buf = %s\r\n", buf);
                strcpy(user_data.account, strtok(buf, "/"));
                strcpy(user_data.password, strtok(NULL, "/"));
                printf("account = %s, password = %s\r\n",
                                user_data.account, user_data.password);
                /* 判断账号的唯一性 */
                if(DoubleLinkedList_Find_Node(user_manager_hand, user_data.account) != NULL)
                {
                    /* 账号存在，注册失败 */
                    memset(buf, 0, sizeof(buf));
                    sprintf(buf, "%s", "error");
                    printf("account");
                    send(accept_sock_fd, buf, strlen(buf), 0);     /* 响应值 */
                    continue;
                }
                memset(buf, 0, sizeof(buf));
                /* 生成4位随机数，发送至客户端 */
                srand(time(NULL));
                int code = rand() % 10000;  /* 0 --- 9999 */
                sprintf(buf, "%04d", code);
                send(accept_sock_fd, (void *)buf, strlen(buf), 0);
                printf("code = %s\r\n", buf);

               
            }
            else if(strcmp(recv_buf, CLINET_GET_USER_INFO) == 0)   /* 验证账号和密码 */
            {
                /************************
                 * 
                 *      服务器先回复一个状态码
                 *    用于反应没有此用户和是否存在输入错误
                 *      -1   ---> 表示没有该用户
                 *      -2   ---> 表示输入密码错误
                 *       0   ---> 表示输入正确，验证通过，给客户端发送用户信息
                 * *********************/
                printf("get user info\r\n");
                /* 接收来自客户端发送账号和密码 */
                char buf[20] = {0};
                char user_account[12] = {0};
                char user_password[7] = {0};
                if(recv(accept_sock_fd, buf, sizeof(buf), 0) < 0)
                {
                    perror("recv account error");
                    break;
                }
                // printf("buf = %s", buf);
                strcpy(user_account, strtok(buf, "/"));
                strcpy(user_password, strtok(NULL, "/"));
                // printf("account = %s, password = %s\r\n", user_account, user_password);
                /* 查询用户信息列表 */
                user_info_t* user_data = NULL;
                int ret = 0;
                user_data = DoubleLinkedList_Find_Node(user_manager_hand, 
                                user_account);
                if(user_data == NULL)
                {
                    printf("can not find account");
                    ret = -1;   /* 没有账号 */
                    send(accept_sock_fd, &ret, sizeof(int), 0);     /* 响应值 */
                }
                if(strcmp(user_password, user_data->password) == 0)
                {
                    /* 发送检验码 */
                    ret = 0;
                    send(accept_sock_fd, (void*)&ret, sizeof(ret), 0);
                    /* 发送用户信息 */
                    send(accept_sock_fd, (void *)user_data, sizeof(user_info_t), 0);
                }
                else
                {
                    ret = -2;
                    send(accept_sock_fd, (void*)&ret, sizeof(ret), 0);
                }
            }
            else if(strcmp(recv_buf, CLINET_REGISTER_USER) == 0)
            {
                /* 注册用户信息 */
                /* 向用户数据链表写入数据 */

                printf("account = %s\r\n", user_data.account);
                write_user_data_file(USER_DATA_DIR, user_data.account, &user_data);
                /* 将数据写入链表 */
                DoubleLinkedList_TailInsert(user_manager_hand, user_data);
            }
            else if(strcmp(recv_buf, CLIENT_BUY_TICKET) == 0)
            {
                printf("buy ticket\r\n");
                /* 买票请求 */
                /* 发送剧场的数据 */
                theater_temp = tcp_theater_send_data(theater_manager_hand, accept_sock_fd);
                if(theater_temp == NULL)
                {
                    printf("can not find theater\r\n");
                    continue;
                }
            }
            else if(strcmp(recv_buf, CLIENT_GET_TICKET) == 0)
            {
                /* 获取票请求 */
                /* 发送对应的剧照和数据 */
                /* 先发送图片 */

            }
            else if(strcmp(recv_buf, CLIENT_GET_SEAT_DATA) == 0)
            {
                printf("get seat data\r\n");
                if(theater_temp == NULL)
                {
                    printf("error theater data\r\n");
                    continue;
                }
                /* 接收终端发送的时间数据 */
                char buf[12] = {0};
                if(recv(accept_sock_fd, buf, sizeof(buf), 0) < 0)
                {
                    perror("recv time error");
                    continue;
                }
                printf("time = %s\r\n", buf);
                /* 打开剧场数据指定文件 */
                char file_path[128] = {0};
                sprintf(file_path, "%s/%s", theater_temp->seat_number_path, buf);

                int open_fd = open(file_path, O_RDWR);
                if(open_fd < 0)
                {
                    perror("open file error");
                    continue;
                }
                char temp_buf[128] = {0};
                while(read(open_fd, temp_buf, sizeof(temp_buf)) > 0)
                {
                    printf("temp_buf = %s\r\n", temp_buf);
                    send(accept_sock_fd, (void *)temp_buf, strlen(temp_buf), 0);
                }
                /* 接收用户的选择数据 */
                memset(temp_buf, 0, sizeof(temp_buf));
                if(recv(accept_sock_fd, temp_buf, sizeof(temp_buf), 0) < 0)
                {
                    printf("recv error\r\n");
                    continue;
                }
                printf("temp_buf = %s\r\n", temp_buf);
                lseek(open_fd, 0, SEEK_SET);
                write(open_fd, temp_buf, strlen(temp_buf));
                close(open_fd);
            }
            else if (strcmp(recv_buf, CLIENT_SYS_USER_DATA) == 0)
            {
                /* 同步用户数据 */
                printf("sync user data\r\n");
                /* 接收客户端发送的用户信息 */
                user_info_t user_data;
                user_info_t *user_info = NULL;
                printf("0\r\n");
                // memset(&user_data, 0, sizeof(user_info_t));
                recv(accept_sock_fd, &user_data, sizeof(user_info_t), 0);
                /* 打印用户数据 */
                printf("user tick = %d\r\n", user_data.ticket_number);
                /* 查找用户数据  */
                printf("user account = %s\r\n", user_data.account);
                printf("user password = %s\r\n", user_data.password);
                user_info = DoubleLinkedList_Find_Node(user_manager_hand, 
                                user_data.account);
                if(user_info == NULL)
                {
                    printf("can not find user data\r\n");
                    continue;
                }
                printf("1\r\n");
                /* 对数据进行赋值 */
                user_info->ticket_number = user_data.ticket_number;
                for(int i = 0; i < user_data.ticket_number; i++)
                {
                    printf("2\r\n");
                    strcpy(user_info->user_tickets[i].play_name, user_data.user_tickets[i].play_name);
                    strcpy(user_info->user_tickets[i].price ,user_data.user_tickets[i].price);
                    strcpy(user_info->user_tickets[i].time_start, user_data.user_tickets[i].time_start);
                    user_info->user_tickets[i].seat_number_x = user_data.user_tickets[i].seat_number_x;
                    user_info->user_tickets[i].seat_number_y = user_data.user_tickets[i].seat_number_y;
                    user_info->user_tickets[i].theater_number = user_data.user_tickets[i].theater_number;
                }
                /* 将数据写入文件 */
                write_user_data_file(USER_DATA_DIR, user_info->account, user_info);

                /* 数据输出 */
                printf("user tick = %d\r\n", user_info->ticket_number);
                for(int i = 0; i < user_info->ticket_number; i++)
                {
                    printf("play_name = %s\r\n", user_info->user_tickets[i].play_name);
                    printf("price = %s\r\n", user_info->user_tickets[i].price);
                    printf("time_start = %s\r\n", user_info->user_tickets[i].time_start);
                    printf("time_play = %s\r\n",user_info->user_tickets[i].time_play);
                    printf("seat_number_x = %d\r\n", user_info->user_tickets[i].seat_number_x);
                    printf("seat_number_y = %d\r\n", user_info->user_tickets[i].seat_number_y);
                    printf("theater_number = %d\r\n", user_info->user_tickets[i].theater_number);
                }
                // memset(&user_data, 0, sizeof(user_info_t));
                // /* 从文件中读取 */
                // read_user_data_file(USER_DATA_DIR, user_info->account, &user_data);

                // printf("tick = %d\r\n", user_data.ticket_number);
                // printf("tick name = %s\r\n", user_data.user_tickets[user_data.ticket_number -1].play_name);
            }
            else
            {
                usleep(100 * 1000);
                continue;
            }
        }
    }
    
}


/*****************************************************
 * 
 *              遍历剧场节点，打开指定的图片
 *      计算该图片的大小，使用tcp向客户端发送图片数据
 *              之后发送剧场结构体
 * 
 * ***************************************************/

theater_info_t* tcp_theater_send_data(theater_head_node_t *Pmanager, 
                int accept_sock_fd)
{
    /* 遍历链表，查找账号名是否与之匹配 */
    //1.为了防止链表中首结点地址丢失，所以定义指针变量对首结点地址进行备份s
    theater_node_t * p = Pmanager->Head;
    FILE* open_image_fd = NULL;
    char recv_buf[50] = {0};
    /* 循环遍历链表，发送图片数据 */
    while(p != NULL)
    {
        /* 发送剧场数据 */
        send(accept_sock_fd, (void*)&p->data, sizeof(theater_info_t), 0);
        printf("send theater data\r\n");
        // printf("p->data.play_name = %s\r\n", p->data.play_name);
        /* 监听客户端的发送请求 */  
        memset(recv_buf, 0, sizeof(recv_buf));
        if(recv(accept_sock_fd, recv_buf, sizeof(recv_buf), 0) < 0)
        {
            perror("recv error");
            return NULL;
        }
        if(strcmp(recv_buf, CLIENT_SEL_NEXT) == 0)
        {
            /* 用户点击下一个 */
            p = p->next;    /* 向下遍历 */

            if(p == NULL)
            {
                p = Pmanager->Head; /* 重新指向第一个 */
                printf("=====\r\n");
            }
               
        }
        else if(strcmp(recv_buf, CLIENT_SEL_PREV) == 0)
        {
            /* 用户点击上一个 */
            p = p->prev;    /* 向上遍历 */
            if(p == NULL)
                p = Pmanager->Head; /* 重新指向第一个 */
        }
        else if(strcmp(recv_buf, CLIENT_EXIT) == 0)
        {
            /* 用户点击退出 */
            /* 返回当前剧场数据 */
            printf("exit \r\n");
            return &p->data;
        }
    }
}

int tcp_send_pictrue(int sockfd, theater_info_t *data)
{
    int   len;
    char  buffer[1024] = {0};
    FILE *fq;

    if( ( fq = fopen(data->picture_path,"rb") ) == NULL )
    {
        printf("File open.\n");
        exit(1);
    }

    bzero(buffer,sizeof(buffer));
    int image_size = 0;
    fseek(fq,0,SEEK_END);
    image_size = ftell(fq);

    send(sockfd, (void*)&image_size, sizeof(image_size), 0);

    char image[image_size];
    memset(image,0,sizeof(image));
    fseek(fq,0,SEEK_SET);

    fread(image,sizeof(image),1,fq);

    send(sockfd, image,sizeof(image),0);

    fclose(fq);

    return 0;
}


