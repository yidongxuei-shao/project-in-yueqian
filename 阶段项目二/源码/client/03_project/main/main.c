#include "main.h"
/*************************************************
 * 
 *                  主线程
 *      初始化硬件
 *      管理多个子线程，接收各个线程的发送的消息
 * 
 * ************************************************/

int tcp_socket_fd;
int udp_sock_fd;
weather_data_t weather_data;

pthread_t thread_user_operation;        /* 用户操作线程 */
lcd_info_t* lcd;

int is_login = 0;

/***************************************
 * 
 *      用户数据结构体变量
 * 
 * ************************************/
user_info_t user_info;
/************************************
 * 
 *      剧场数据结构体变量
 * 
 * **********************************/
theater_info_t theater_info;
int main(void)
{
    /* 初始化lcd */
    lcd = lcd_init();
    if(lcd == NULL)
    {
        printf("lcd init failed\n");
        return -1;
    }

    /* 创建线程显示开机动画，系统初始化完成，结束动画，进入待机界面 */
    // pthread_t thread_start;
    // if(pthread_create(&thread_start, NULL, thread_start_animation, (void*)&lcd) != 0)
    // {
    //     printf("create thread start failed\n");
    //     return -1;
    // }
    udp_sock_fd = udp_init();
    if(udp_sock_fd < 0)
    {
        printf("udp init failed\n");
        return -1;
    }
    /* 连接服务器 */
    tcp_socket_fd = tcp_connect(SERVER_IP, SERVER_PORT);
    if(tcp_socket_fd < 0)
    {
        printf("connect server failed\n");
        /* 无法连接服务器，则跳过 */
        goto end_stmove;
    }
    /* 校准时间 */
    if(tcp_send(tcp_socket_fd, CLIENT_GET_TIME, strlen(CLIENT_GET_TIME)) < 0)
    {
        printf("send get time failed\n");
        return -1;
    }
    char time_buf[60] = {0};
    if(tcp_recv(tcp_socket_fd, (void*)&time_buf, sizeof(time_buf)) < 0)
    {
        printf("recv get time failed\n");
        return -1;
    }

    printf("time buf = %s\r\n", time_buf);
    /* 切割字符获取年月日，时分秒 */
    char* year = strtok(time_buf, "/");
    char* hour = strtok(NULL, "/");
    /* 修改系统年月日 */
    char temp[30] = {0};
    // bzero(time_buf, sizeof(time_buf));
    sprintf(temp,"date -s %s", year);
    system(temp);
    /* 修改系统时分秒 */
    bzero(temp, sizeof(temp));
    sprintf(temp,"date --set %s", hour);
    system(temp);
    printf("system clock set ok\r\n");

    /* 获取天气信息 */
    tcp_get_weather_data(tcp_socket_fd, &weather_data);

    /* 输出天气信息 */
    // printf("temp = %d, text = %s\r\n", weather_data.temp, weather_data.weather);
end_stmove:     /* 等待结束开机动画 */
    /* 结束开机动画 */
    printf("system inital compared\r\n");
    // sleep(5);       /* 给开机动画播放5s */
    /* 结束开机动画线程 */
    // pthread_cancel(thread_start);
    // pthread_kill(thread_start, SIGUSR1);
    pthread_t thread_waitting;
watting_interface:
#if 1
    /* 进入待机界面 */
    if(pthread_create(&thread_waitting, NULL, thread_start_video, (void*)lcd) != 0)
    {
        printf("create thread waitting failed\n");
        return -1;
    }
    usleep(500 * 1000);       /* 等待待机界面显示完毕 */
    /* 创建获取触摸屏线程 */
    pthread_t thread_touch;
    if(pthread_create(&thread_touch, NULL, touch_thread, NULL) != 0)
    {
        printf("create thread touch failed\n");
        return -1;
    }

    /* 等待有人按下触摸屏，退出待机界面，进入用户操作界面 */
    while(touch_flag != 1)
    {
        sleep(1);
    }
    /* 用户按下触摸屏 */
    printf("touch down\r\n");
    /* 清空触摸坐标 */
    touch_flag = 0;
    touch_x = 0;
    touch_y = 0;
    /* 结束播放器 */
    mplayer_stop();
    /* 清空终端 */
    system("clear");
    /* 结束待机线程 */
    pthread_kill(thread_waitting, SIGUSR1);
    printf("mplayer stop ok\r\n");
    

    /* 创建线程，用于捕获按键事件 */
    pthread_t thread_main_key;
    if(pthread_create(&thread_main_key, NULL, task_user_key_main, (void*)lcd) != 0)
    {
        printf("create thread main key failed\n");
        return -1;
    }
    /* 进入用户操作界面 */
    thread_user_operation = pthread_self();
    int user_ret = show_user_operation(lcd);
    if(user_ret == BREAK_LAST)
    {
        printf("break last\r\n");
        goto watting_interface;
    }
    printf("ok\r\n");
    /* 等待用户操作线程结束 */
    printf("main key thread end, ret = %d\r\n", user_ret_val);
    /* 判断线程的返回值 */
    int ret_login = 0;
    switch (user_ret_val)
    {
        case GET_TICKS:
        {
            printf("show login\r\n");
            ret_login = show_login(lcd);
            if(ret_login != GOTO_NEXT)
                goto watting_interface;     /* 回到待机界面 */
            /* 显示用户中心界面 */
            int ret = show_user_center(lcd);
            if(ret == GOTO_NEXT)
            {
                /* 前往买票 */
                goto goto_buy_ticks;
            }
            else
            {
                /* 已经取票完成，返回待机界面 */
                goto watting_interface;
            }
            break;
        }
        case BUY_TICKS:
        {
goto_buy_ticks:            
            printf("show buy ticks\r\n");
            /* 显示选票界面 */
            /* 给服务器发送买票请求 */
            if(tcp_send(tcp_socket_fd, CLIENT_BUY_TICKET, strlen(CLIENT_BUY_TICKET)) < 0)
            {
                printf("tcp send error\r\n");
                return -1;
            }
            tcp_get_theater_data(tcp_socket_fd, 1, 0, 0);
            if(show_select_ticket(lcd, &theater_info, tcp_socket_fd) < 0)
            {
                printf("show select ticket error\r\n");
                return -1;
            }
            /* 显示支付界面 */
            if(show_pay(lcd, tcp_socket_fd, &theater_info, &user_info) == 0)
            {
                /* 回到待机界面 */
                goto watting_interface;
            }
            break;
        }
        case SHOPPING:
            /* 显示界面施工中 */
            show_picture(lcd, IMAGE_UNDO, 1, 0, 0);
            for(int i = 10; i >= 0; i--)
            {
                /* 在指定的位置显示数字 */
                char buf[2] = {0};
                sprintf(buf, "%d", i);
                show_font_number(lcd,UNDO_COUNTDOWN_X, 
                    UNDO_COUNTDOWN_Y, buf,32,BLACK,32);
                sleep(1);
            }
            goto watting_interface;
        default:
            break;
    }
#endif
    printf("process exit\r\n");
    while (1);
    return 0;
}
