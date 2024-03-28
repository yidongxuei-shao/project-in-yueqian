#include "gui.h"

char user_account[13] = {0};        /* 用于保存用户账号 */
char user_password[13] = {0};       /* 用于保存用户密码 */

void show_time(lcd_info_t* lcd, int x, int y)
{
    /* 获取当前时间 */
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char time_buf[50] = {0};
    sprintf(time_buf, "%04d年%02d月%02d日", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    /* 调用字库显示 年月日*/
    show_font(lcd, x, y,time_buf, BLACK, 32);
    /* 清空缓冲 */
    memset(time_buf, 0,sizeof(time_buf));
    /* 显示时分 */
    sprintf(time_buf, "%02d:%02d", tm->tm_hour, tm->tm_min);
    printf("time buf = %s\r\n", time_buf);
    show_font(lcd, x + 40, y + 40,time_buf, BLACK, 40);
}


/***************************
 * 
 *      显示天气数据
 *  
 * ************************/
int show_weather(lcd_info_t* lcd, weather_data_t* weather ,int x, int y)
{
    /* 有效性判断 */
    if(x < 0 || y < 0 || x > (800 - 48) || y > (480 - 48))
    {
        printf("show weather: input argument error\r\n");
        return -1;
    }
    char temp[40] = {0};
    strcpy(temp, weather->weather);
    /* 删除weather_name内的空格 */
    deleteChar(' ', temp);
    printf("temp = %s\r\n", temp);
    /* 读取指定的文件的内容 */
    /* 合成图标路径 */
    char weather_icon_path[60] = {0};
    sprintf(weather_icon_path, "%s/%s.png", IMAGE_WEATHER_PATH, temp);
    printf("weather_icon_path = %s\r\n", weather_icon_path);
    /* 显示天气图标 */
    read_png_file(x + 30, y, weather_icon_path, lcd);
    /* 显示文字 */
    show_font(lcd, x + 80, y, weather->weather, BLACK, 32);

    /* 显示温度 */
    memset(temp, 0, sizeof(temp));
    sprintf(temp, "当前温度：%d", weather->temp);
    show_font(lcd, x + 60, y + 55, temp, BLACK, 32);
    return 0;
}

/*******************************************
 * 
 *        将客户端的数据发送给服务器
 *      服务器再将验证结果发送给客户端
 *      
 * ****************************************/
int tcp_send_user_data(int sockfd, char *account, char *password)
{
    if(sockfd < 0 || account == NULL || password == NULL)
    {
        printf("tcp_send_user_data:invalid parameter\n");
        return -1;
    }
    printf("account = %s, password = %s\r\n", account, password);
    /* 发送用户账号和密码 */
    char buf[20] = {0};
    sprintf(buf, "%s/%s", account, password);
    if(tcp_send(sockfd, buf, strlen(buf)) < 0)
    {
        printf("send account failed\n");
    } 
    return 0;
}


/****************************************************
 * 
 *              显示用户操作界面
 *          能循环播放不同的广告图片
 *          当用户按下功能按键时能响应
 * **************************************************/

/************
 * 
 *      信号响应接口     
 * 
 * **********/
int is_user_end = 0;
void signal_user_handler(int sig)
{
   
    if(sig == SIGUSR1)
    {
        is_user_end = 1;
    }
}
int show_user_operation(lcd_info_t* lcd)
{
    /* 有效性判断 */
    if(lcd == NULL)
    {
        printf("show user operation: input argument error\r\n");
        return -1;
    }
    /* 注册信号响应接口 */
    // pthread_detach(pthread_self());
    signal(SIGUSR1, signal_user_handler);
    /* 显示用户操作界面 */
    show_picture(lcd, IMAGE_MAIN, 1, 0, 0);
   
    /* 在指定的位置播放不同的广告图片 */
    int advist_num = 0;
    int count = 0;
    char** advist_name = NULL;
    dir_file(IMAGE_AD_PATH, ".jpg", &advist_num, &advist_name);
    // printf("ad = %s\r\n", advist_name[0]);
    char buf[100] = {0};
    while(1)
    {
        for(int i = 0; i < advist_num; i++)
        {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s/%s", IMAGE_AD_PATH, advist_name[i]);
            // printf("buf = %s\r\n", buf);
            show_picture(lcd, buf, 1, 20, 150);
            sleep(10);
            count ++;
            if(count >= 3*10)     /* 300s */
            {
                printf("user operation: exit\r\n");
                return BREAK_LAST;
            }
            if(is_user_end == 1)
            {
                is_user_end = 0;
                return GOTO_NEXT;
            }
        }
    }
}

/*******************************************
 * 
 *          显示登录界面
 *      当用户按下了取票时显示
 *        捕获用户按下的位置
 * ****************************************/
int show_login(lcd_info_t* lcd)
{
    /* 有效性判断 */
    if(lcd == NULL )
    {
        printf("show login: input argument error\r\n");
        return -1;
    }
    char temp;
    int is_ok_accout = 0;
    int is_ok_password = 0;
    char buf[2] = {0};
    int err_num = 0;
    /* 显示登录界面 */
    show_picture(lcd, IMAGE_LOGIN, 1, 0, 0);

    while(1)
    {
       
        if(touch_flag == 0)
        {
            usleep(100 * 1000);
            continue;
        }
        touch_flag = 0;
        /* 判断用户按下的位置 */
        printf("touch x = %d, touch y = %d\r\n", touch_x, touch_y);
        /* 当用户按下账号输入框，则开始输入账号 */
        if(touch_x > BUTTON_LOGIN_ACCOUNT_INPUT_X && touch_x < (BUTTON_LOGIN_ACCOUNT_INPUT_X + BUTTON_LOGIN_USER_WIDTH) 
                && touch_y > BUTTON_LOGIN_ACCOUNT_INPUT_Y && touch_y < (BUTTON_LOGIN_ACCOUNT_INPUT_Y + BUTTON_LOGIN_USER_HEIGHT))
        {
            printf("1\r\n");
            show_number_lcd(lcd, BUTTON_LOGIN_ACCOUNT_INPUT_X, 
                    BUTTON_LOGIN_ACCOUNT_INPUT_Y, user_account, USER_ACCOUNT_LEN);
            /* 标志位最低位写1 */
            is_ok_accout = 1;
            printf("account = %s\r\n", user_account);
        }
        else if(touch_x > BUTTON_LOGIN_PASSWORD_INPUT_X && touch_x < BUTTON_LOGIN_PASSWORD_INPUT_X 
                        + BUTTON_LOGIN_USER_WIDTH - BUTTON_VISIBLE_WIDTH
                && touch_y > BUTTON_LOGIN_PASSWORD_INPUT_Y && touch_y < BUTTON_LOGIN_PASSWORD_INPUT_Y 
                        + BUTTON_LOGIN_USER_HEIGHT)
        {
            /* 输入密码 */
            printf("password\r\n");
            if(is_ok_accout != 1)
            {
                printf("input account first\r\n");
                continue;
            }
            show_number_lcd(lcd, BUTTON_LOGIN_PASSWORD_INPUT_X, 
                    BUTTON_LOGIN_PASSWORD_INPUT_Y, user_password, USER_PASSWORD_LEN);
            /* 输出密码 */
            printf("password = %s\r\n", user_password);
            is_ok_password = 1;
        }
        else if(touch_x > BUTTON_LOGIN_USER_X && touch_x < BUTTON_LOGIN_USER_X + BUTTON_LOGIN_WIDTH
                && touch_y > BUTTON_LOGIN_USER_Y && touch_y < BUTTON_LOGIN_USER_Y + BUTTON_LOGIN_HEIGHT)
        {
            /* 按下登录按键 */
            printf("user login\r\n");
            is_login = 0;
            /* 检测是否有输入 */
            if(is_ok_accout != 1 || is_ok_password != 1)
            {
                printf("input error\r\n");
                /* 清空输入 */
                show_picture(lcd, IMAGE_LOGIN_CLEAR, 1, BUTTON_LOGIN_ACCOUNT_INPUT_X,
                                BUTTON_LOGIN_ACCOUNT_INPUT_Y);
                show_picture(lcd, IMAGE_LOGIN_CLEAR, 1, BUTTON_LOGIN_PASSWORD_INPUT_X,
                                BUTTON_LOGIN_PASSWORD_INPUT_Y);
                memset(user_account, 0, sizeof(user_account));
                memset(user_password, 0, sizeof(user_password));
                continue;
            }
             /* 向服务器发送验证账号和密码的请求码 */
            if(tcp_send(tcp_socket_fd, CLIENT_GET_USER_INFO, 
                                strlen(CLIENT_GET_USER_INFO))< 0)/* 向服务器发送信息 */
            {
                printf("send CLINET_GET_USER_INFO error\r\n");
                return -1;
            }
            /* 验证账号和密码 */
            /* 清空账号位和密码位 */
            is_ok_accout = 0;
            is_ok_password = 0;
            tcp_send_user_data(tcp_socket_fd, user_account, user_password);

            /* 接收服务端的返回值 */
            int ret_val = -4;
            if(tcp_recv(tcp_socket_fd, (void*)&ret_val, sizeof(int)) < 0)
            {
                printf("tcp recv error\r\n");
                return -1;
            }
            printf("ret_val = %d\r\n", ret_val);
            if(ret_val == -1)   /* 没有找到该用户 */
            {
                /* 输入错误 */
                printf("login user error\r\n");
                 /* 清空输入 */
                show_picture(lcd, IMAGE_LOGIN_CLEAR, 1, BUTTON_LOGIN_ACCOUNT_INPUT_X,
                                BUTTON_LOGIN_ACCOUNT_INPUT_Y);
                show_picture(lcd, IMAGE_LOGIN_CLEAR, 1, BUTTON_LOGIN_PASSWORD_INPUT_X,
                                BUTTON_LOGIN_PASSWORD_INPUT_Y);
                memset(user_account, 0, sizeof(user_account));
                memset(user_password, 0, sizeof(user_password));
                err_num ++;
                if(err_num >= 3)
                   return BREAK_LAST;
                continue;
            }
            else if (ret_val == -2)
            {
                /* 密码输入错误 */
                /* 清空输入 */
                show_picture(lcd, IMAGE_LOGIN_CLEAR, 1, BUTTON_LOGIN_PASSWORD_INPUT_X,
                                BUTTON_LOGIN_PASSWORD_INPUT_Y);
                memset(user_password, 0, sizeof(user_password));
                err_num ++;
                if(err_num >= 3)
                   return BREAK_LAST;
                continue;
            }
            else if( ret_val == 0)
            {
                printf("login user sucessful\r\n");
                /* 清空用户数据 */
                // memset(&user_info, 0, sizeof(user_info_t));    
                /* 接收用户数据 */
                tcp_recv(tcp_socket_fd, (void*)&user_info, sizeof(user_info_t));
                printf("user_info = %s\r\n", user_info.account);
                is_login = 1;
                return GOTO_NEXT;
            }
            else
            {
                /* 登录失败 */
                printf("login user error\r\n");
                /* 清空输入 */
                show_picture(lcd, IMAGE_LOGIN_CLEAR, 1, BUTTON_LOGIN_ACCOUNT_INPUT_X,
                                BUTTON_LOGIN_ACCOUNT_INPUT_Y);
                show_picture(lcd, IMAGE_LOGIN_CLEAR, 1, BUTTON_LOGIN_PASSWORD_INPUT_X,
                                BUTTON_LOGIN_PASSWORD_INPUT_Y);
                memset(user_account, 0, sizeof(user_account));
                memset(user_password, 0, sizeof(user_password));
                continue;
            }
        }
        else if(touch_x > BUTTON_LOGIN_REGISTER_X && touch_x < BUTTON_LOGIN_REGISTER_X + BUTTON_LOGIN_WIDTH
            && touch_y > BUTTON_LOGIN_REGISTER_Y && touch_y < BUTTON_LOGIN_REGISTER_Y + BUTTON_LOGIN_HEIGHT)
        {
            /* 用户按下注册 */
            printf("user register\r\n");
            /* 显示用户注册界面 */
            int ret = show_register_lcd(lcd);
            if(ret == BREAK_LAST || ret == GOTO_NEXT)
            {
                /* 显示登录界面 */
                show_picture(lcd, IMAGE_LOGIN, 1, 0, 0);
                continue;     
            }
        }
        else if(touch_x > BUTTON_LOGIN_ADMIN_X && touch_x < BUTTON_LOGIN_ADMIN_X + BUTTON_LOGIN_ADMIN_WIDTH
            && touch_y > BUTTON_LOGIN_ADMIN_Y && touch_y < BUTTON_LOGIN_ADMIN_Y + BUTTON_LOGIN_ADMIN_HEIGHT)
        {
            /* 用户按下管理员 */
            printf("user admin\r\n");
        }
        else if(touch_x > BUTTON_LOGIN_VISIBLE_X && touch_x < BUTTON_LOGIN_VISIBLE_X + BUTTON_VISIBLE_WIDTH
            && touch_y > BUTTON_LOGIN_VISIBLE_Y && touch_y < BUTTON_LOGIN_VISIBLE_Y + BUTTON_VISIBLE_HEIGHT)
        {
            /* 用户按下可见 */
            printf("user visible\r\n");
        }
        else if(touch_x > BUTTON_BACK_X && touch_x < BUTTON_BACK_X + BUTTON_BACK_WIDTH
            && touch_y > BUTTON_BACK_Y && touch_y < BUTTON_BACK_Y + BUTTON_BACK_HEIGHT)
        {
            /* 用户按下返回 */
            printf("user back\r\n");
            return -1;
            break;
        }
        else
        {
         
            touch_x = 0;
            touch_y = 0;
            continue;
        }
        /* 清空触摸值 */
        touch_x = 0;
        touch_y = 0;
    }
}


/******************************
 * 
 *      在lcd屏上显示数字
 *      并获取输入的数字
 * ****************************/
int show_number_lcd(lcd_info_t* lcd, int x, int y, char* output_data, int data_len)
{
    char buf[2] = {0};
    char temp = 0;
    /* 输入账号 */
    /* 获取键盘的输入 */
    for(int i = 0; i < data_len; )
    {
        temp = get_user_keypad_intput();
        touch_x = 0;
        touch_y = 0;
        printf("temp = %c\r\n", temp);
        if(temp == 'd')
        {
            if(i == 0)
            {
                i = 0;
                printf("posstion start\r\n");
                continue;
            }
            else
            {
                i--;
                printf("del one input again\r\n");

                /* 使用空格覆盖文字 */
                show_font_number(lcd, x + i * 24 + 10, 
                        y + 10, " ", 24, BLACK, 24);
                continue;
            }
        }
        if(temp == 'b')
        {
            return -1;
        }
        /* 将字符写入到数组中 */
        output_data[i] = temp;
        sprintf(buf, "%c", temp);
        
        /* 将字符写到lcd屏中 */
        show_font_number(lcd, x + i * 24 + 10, 
                    y + 10, buf, 24, BLACK, 24);
        i++;
    }
}


/***********************************
 * 
 *      显示注册界面
 * 
 * ********************************/
int show_register_lcd(lcd_info_t* lcd)
{
    char user_account[13] = {0};
    char user_password[7] = {0};
    char user_code[6] = {0};
    char user_input_code[6] = {0};
    int err_num = 0;
    int is_ok = 0;
    /* 显示注册界面 */
    show_picture(lcd, IMAGE_REGISTER, 1, 0, 0);
    while(1)
    {
        if(touch_flag == 0)
        {
            usleep(100 * 1000);
            continue;
        }

        touch_flag = 0;

        if(touch_x > BUTTON_BACK_X && touch_x < BUTTON_BACK_X + BUTTON_BACK_WIDTH
            && touch_y > BUTTON_BACK_Y && touch_y < BUTTON_BACK_Y + BUTTON_BACK_HEIGHT)
        {
            /* 用户按下返回 */
            printf("back\r\n");
            touch_x = 0;
            touch_y = 0;
            return BREAK_LAST;
        }
        else if(touch_x > BUTTON_REGISTER_SEND_X && touch_x < BUTTON_REGISTER_SEND_X + BUTTON_REGISTER_SEND_WIDTH
            && touch_y > BUTTON_REGISTER_SEND_Y && touch_y < BUTTON_REGISTER_SEND_Y + BUTTON_REGISTER_SEND_HEIGHT)
        {
            /* 按下发送按键 */
            if((is_ok & 0x02) != 0x02)
            {
                /* 输入错误 */
                printf("register fail\r\n");
                show_picture(lcd, IMAGE_REGISTER_ACC_CLEAR, 1,
                         REGISTER_PHONE_INPUT_X, REGISTER_PHONE_INPUT_Y);
                show_picture(lcd, IMAGE_REGISTER_ACC_CLEAR, 1,
                         REGISTER_PASSWORD_INPUT_X, REGISTER_PASSWORD_INPUT_Y);
                show_picture(lcd, IMAGE_REGISTER_CODE_CLEAR, 1,
                         REGISTER_CODE_INPUT_X, REGISTER_CODE_INPUT_Y);
                touch_x = 0;
                touch_y = 0;
                /* 清空数组 */
                memset(user_account, 0, sizeof(user_account));
                memset(user_password, 0, sizeof(user_password));
                continue;
            }
            /* 服务器发送验证码请求 */
            tcp_send(tcp_socket_fd, CLIENT_GET_CODE, strlen(CLIENT_GET_CODE));
            printf("send code\r\n");
            /* 发送用户的账号和密码 */
            /* 将用户的账号和密码发送给服务器 */
            char buf[30] = {0};
            sprintf(buf, "%s/%s", user_account, user_password);
            if(tcp_send(tcp_socket_fd, (void*)&buf, sizeof(buf)) < 0)
            {
                printf("get code error\r\n");
                continue;
            }
            /* 接收服务器的验证码 */
            if(tcp_recv(tcp_socket_fd, (void*)&user_code, sizeof(user_code)) < 0)
            {
                printf("get code error\r\n");
                memset(user_code, 0, sizeof(user_code));
                touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(strcmp(user_code, "error") == 0)
            {
                printf("get code error\r\n");
                memset(user_code, 0, sizeof(user_code));
                /* 清空输入框 */
                show_picture(lcd, IMAGE_REGISTER_ACC_CLEAR, 1,
                         REGISTER_PHONE_INPUT_X, REGISTER_PHONE_INPUT_Y);
                show_picture(lcd, IMAGE_REGISTER_ACC_CLEAR, 1,
                         REGISTER_PASSWORD_INPUT_X, REGISTER_PASSWORD_INPUT_Y);
                show_picture(lcd, IMAGE_REGISTER_CODE_CLEAR, 1,
                         REGISTER_CODE_INPUT_X, REGISTER_CODE_INPUT_Y);
                touch_x = 0;
                touch_y = 0;
                continue;
            }
            printf("user_code = %s\r\n", user_code);
            touch_x = 0;
            touch_y = 0;
        }
        else if(touch_x > BUTTON_REGISTER_VISIBLE_X && touch_x < BUTTON_REGISTER_VISIBLE_X + BUTTON_VISIBLE_WIDTH
            && touch_y > BUTTON_REGISTER_VISIBLE_Y && touch_y < BUTTON_REGISTER_VISIBLE_Y + BUTTON_VISIBLE_HEIGHT)
        {
            /* 用户按下可见 */
        }
        else if(touch_x > BUTTON_REGISTER_X && touch_x < BUTTON_REGISTER_X + BUTTON_REGISTER_WIDTH
            && touch_y > BUTTON_REGISTER_Y && touch_y < BUTTON_REGISTER_Y + BUTTON_REGISTER_HEIGHT)
        {
            /* 用户按下注册 */
            printf("register\r\n");
            /* 确认验证是否正确 */
            if(strcmp(user_code, user_input_code) != 0)
            {
               /* 验证失败 */
               printf("register fail\r\n");
               err_num ++;
               /* 重新发送验证码 */
               tcp_send(tcp_socket_fd, CLIENT_GET_CODE, strlen(CLIENT_GET_CODE));
               if(tcp_recv(tcp_socket_fd, (void*)&user_code, sizeof(user_code)) < 0)
               {
                   printf("get code error\r\n");
                    touch_x = 0;
                    touch_y = 0;
                   continue;
               }
               if(err_num >= 3)
               {
                   printf("register fail 3 times\r\n");
                    touch_x = 0;
                    touch_y = 0;
                   return BREAK_LAST;
               }
               continue;
            }
            /* 注册成功 */
            printf("register success\r\n");
            /* 向服务器发送注册账号请求 */
            tcp_send(tcp_socket_fd, CLIENT_REGISTER_USER, strlen(CLIENT_REGISTER_USER));

            
            return GOTO_NEXT;
        }
        else if(touch_x > REGISTER_PHONE_INPUT_X && touch_x < REGISTER_PHONE_INPUT_X + REGISTER_INPUT_WIDTH
            && touch_y > REGISTER_PHONE_INPUT_Y && touch_y < REGISTER_PHONE_INPUT_Y + REGISTER_INPUT_HEIGHT)
        {
            /* 电话号码输入框 */
           int ret = show_number_lcd(lcd, REGISTER_PHONE_INPUT_X, 
                    REGISTER_PHONE_INPUT_Y, user_account, USER_ACCOUNT_LEN);
            if(ret < 0)
               continue;
            is_ok |= 1 << 0;
        }
        else if(touch_x > REGISTER_PASSWORD_INPUT_X && touch_x < REGISTER_PASSWORD_INPUT_X + REGISTER_INPUT_WIDTH
            && touch_y > REGISTER_PASSWORD_INPUT_Y && touch_y < REGISTER_PASSWORD_INPUT_Y + REGISTER_INPUT_HEIGHT)
        {
            /* 密码输入框 */
            int ret = show_number_lcd(lcd, REGISTER_PASSWORD_INPUT_X, 
                    REGISTER_PASSWORD_INPUT_Y, user_password, USER_PASSWORD_LEN);
            if(ret < 0)
               continue;
            is_ok |= 1 << 1;
        }
        else if(touch_x > REGISTER_CODE_INPUT_X && touch_x < REGISTER_CODE_INPUT_X + REGISTER_CODE_INPUT_WIDTH
            && touch_y > REGISTER_CODE_INPUT_Y && touch_y < REGISTER_CODE_INPUT_Y + REGISTER_CODE_INPUT_HEIGHT)
        {
            /* 验证码输入框 */
            printf("code input\r\n");
            int ret = show_number_lcd(lcd, REGISTER_CODE_INPUT_X, 
                    REGISTER_CODE_INPUT_Y, user_input_code, USER_CODE_LEN);
            if(ret < 0)
               continue;
        }
        else
        {
            touch_x = 0;
            touch_y = 0;
            usleep(100 * 1000);
            continue;
        }

    }
}

/**************************
 * 
 *       显示用户中心界面
 * 
 * ***********************/
int show_user_center(lcd_info_t* lcd)
{
    int err_num = 0;
    /* 显示用户中心界面 */
    show_picture(lcd, IMAGE_USER_CENTER, 1, 0, 0);
    int tick_num = 0;
    /* 显示用户的第一个电影票的信息 */
    int ret = show_user_center_tickinfo(tick_num);
    if(ret < 0)
    {
        /* 用户没有票 */
        printf("no ticket\r\n");
    }
    while(1)
    {
        if(touch_flag != 1)
        {
            usleep(100 * 1000);
            continue;
        }
        touch_flag = 0;
        if(touch_x > BUTTON_BACK_X && touch_x < BUTTON_BACK_X + BUTTON_BACK_WIDTH
            && touch_y > BUTTON_BACK_Y && touch_y < BUTTON_BACK_Y + BUTTON_BACK_HEIGHT)
        {
            /* 用户按下返回 */
            printf("back\r\n");
            return BREAK_LAST;
        }
        else if(touch_x > BUTTON_USER_CENTER_GET_X && touch_x < BUTTON_USER_CENTER_GET_X + BUTTON_USER_CENTER_WIDTH
            && touch_y > BUTTON_USER_CENTER_GET_Y && touch_y < BUTTON_USER_CENTER_GET_Y + BUTTON_USER_CENTER_HEIGHT)
        {
            /* 用户按下了取票 */
            if(user_info.ticket_number == 0)
            {
                printf("user has no ticket\r\n");
                continue;
            }
            /* 同步用户的数据 */
            /* 向服务器发送取票请求 */
            if(tcp_send(tcp_socket_fd, CLIENT_GET_TICKET, strlen(CLIENT_GET_TICKET)) < 0)
            {
                printf("tcp send error\r\n");
                continue;
            }
            /* 将当前的票的下标，取出，备份，删除 */
            struct_ticket_t tick_info = user_ticket_delete(&user_info, tick_num);
            /* 打印当前的票 */
            printf("tick_info = %s\r\n", tick_info.play_name);
            /* 同步用户的信息 */
            if(tcp_send(tcp_socket_fd, (void*)&user_info, sizeof(user_info)) < 0)
            {
                printf("tcp recv error\r\n");
                continue;
            }
            return BREAK_LAST;
        }
        else if(touch_x > BUTTON_USER_CENTER_BUY_X && touch_x < BUTTON_USER_CENTER_BUY_X + BUTTON_USER_CENTER_WIDTH
            && touch_y > BUTTON_USER_CENTER_BUY_Y && touch_y < BUTTON_USER_CENTER_BUY_Y + BUTTON_USER_CENTER_HEIGHT)
        {
            /* 用户按下买票 */ 
            /* 服务器发送当前存在的电影的信息、剧照和最近的时间 */
            return GOTO_NEXT;
        }
        else if(touch_x > BUTTON_USER_CENTER_SETTING_X && touch_x < BUTTON_USER_CENTER_SETTING_X + BUTTON_USER_CENTER_SET_WIDTH
            && touch_y > BUTTON_USER_CENTER_SETTING_Y && touch_y < BUTTON_USER_CENTER_SETTING_Y + BUTTON_USER_CENTER_SET_HEIGHT)
        {
            /* 用户按下密码设置 */
        }
        else if(touch_x > BUTTON_USER_CENTER_NEXT_X && touch_x < BUTTON_USER_CENTER_NEXT_X + BUTTON_USER_CENTER_NEXT_WIDTH
            && touch_y > BUTTON_USER_CENTER_NEXT_Y && touch_y < BUTTON_USER_CENTER_NEXT_Y + BUTTON_USER_CENTER_NEXT_HEIGHT)
        {
            tick_num++;
            /* 用户按下下一页 */
            if(tick_num >= user_info.ticket_number)
            {
                /* 不显示 */
                printf("no next page\r\n");
                tick_num = user_info.ticket_number - 1; /* 最后一张票的信息 */
                continue;
            }
            show_user_center_tickinfo(tick_num);
        }
        else if(touch_x > BUTTON_USER_CENTER_PRE_X && touch_x < BUTTON_USER_CENTER_PRE_X + BUTTON_USER_CENTER_PRE_WIDTH
            && touch_y > BUTTON_USER_CENTER_PRE_Y && touch_y < BUTTON_USER_CENTER_PRE_Y + BUTTON_USER_CENTER_PRE_HEIGHT)
        {
            tick_num--;
            /* 用户按下上一页 */
            if(tick_num < 0)
            {
                /* 不显示 */
                printf("no pre page\r\n");
                tick_num = 0;
                continue;
            }
            show_user_center_tickinfo(tick_num);
        }
        else
        {
            usleep(100 * 1000);
        }
    }
}


/***********************************
 * 
 *  显示指定的用户的票数和相关信息
 *     
 * ********************************/
int show_user_center_tickinfo(int num)
{
    /* 显示用户的余额和票数 */
    char user_buf[10] = {0};
    sprintf(user_buf, "%02.2f", user_info.balance);
    show_font(lcd, USER_CENTER_OUTPUT_BALANCE_X, USER_CENTER_OUTPUT_BALANCE_Y, 
                    user_buf, BLACK, 32);
    memset(user_buf, 0, sizeof(user_buf));
    sprintf(user_buf, "%d", user_info.ticket_number);
    show_font(lcd, USER_CENTER_OUTPUT_NUMBER_X, USER_CENTER_OUTPUT_NUMBER_Y, 
                user_buf, BLACK, 32);
    /* 两个都显示 */
    // read_png_file(BUTTON_USER_CENTER_PRE_X, BUTTON_USER_CENTER_PRE_Y,
    //                 IMAGE_USER_CENTER_PREV, lcd);
    // read_png_file(BUTTON_USER_CENTER_NEXT_X, BUTTON_USER_CENTER_NEXT_Y,
    //                 IMAGE_USER_CENTER_NEXT, lcd);
    show_picture(lcd, IMAGE_USER_CENTER_NEXT, 1, BUTTON_USER_CENTER_NEXT_X, BUTTON_USER_CENTER_NEXT_Y);
    show_picture(lcd, IMAGE_USER_CENTER_PREV, 1, BUTTON_USER_CENTER_PRE_X, BUTTON_USER_CENTER_PRE_Y);

    if(user_info.ticket_number == 0)
    {
        /* 用户没有票 */
        /* 再指定的位置显示NULL */
        /* 显示影片的名字 */
        show_font(lcd, USER_CENTER_OUTPUT_MOVE_X, USER_CENTER_OUTPUT_MOVE_Y, 
                    "NULL", BLACK, 24);
        /* 显示价格 */
        show_font(lcd, USER_CENTER_OUTPUT_PRICE_X, USER_CENTER_OUTPUT_PRICE_Y, 
                    "N/A", BLACK, 24);
        /* 显示开始时间 */
        show_font(lcd, USER_CENTER_OUTPUT_START_X, USER_CENTER_OUTPUT_START_Y, 
                    "N/A", BLACK, 24);
        /* 显示剧场标号 */
        show_font(lcd, USER_CENTER_OUTPUT_ROOM_X, USER_CENTER_OUTPUT_ROOM_Y, 
                    "N/A", BLACK, 24);
        /* 显示座位号 */
        show_font(lcd, USER_CENTER_OUTPUT_SEAT_X, USER_CENTER_OUTPUT_SEAT_Y, 
                    "N/A", BLACK, 24);
        return -1;
    }
#if 0
    if(num == 0)
    {
        /* 不显示上一个的图标，显示下一个的图标 */
        read_png_file(BUTTON_USER_CENTER_NEXT_X, BUTTON_USER_CENTER_NEXT_Y,
                        IMAGE_USER_CENTER_NEXT, lcd);
        show_picture(lcd, IMAGE_USER_CENTER_CLEAR, 1, 
                        BUTTON_USER_CENTER_PRE_X, BUTTON_USER_CENTER_PRE_Y);
    }
    else if(num == user_info.ticket_number - 1)
    {
        /* 不显示下一个的图标，显示上一个的图标 */
        read_png_file(BUTTON_USER_CENTER_PRE_X, BUTTON_USER_CENTER_PRE_Y,
                        IMAGE_USER_CENTER_PREV, lcd);
        show_picture(lcd, IMAGE_USER_CENTER_CLEAR, 1, 
                        BUTTON_USER_CENTER_NEXT_X, BUTTON_USER_CENTER_NEXT_Y);
    }
    else
    {
        /* 两个都显示 */
        read_png_file(BUTTON_USER_CENTER_PRE_X, BUTTON_USER_CENTER_PRE_Y,
                        IMAGE_USER_CENTER_PREV, lcd);
        read_png_file(BUTTON_USER_CENTER_NEXT_X, BUTTON_USER_CENTER_NEXT_Y,
                        IMAGE_USER_CENTER_NEXT, lcd);
    }
#endif

    /* 调用字库再指定的位置显示信息 */
    /* 显示影片的名字 */
    show_font(lcd, USER_CENTER_OUTPUT_MOVE_X, USER_CENTER_OUTPUT_MOVE_Y, 
                user_info.user_tickets[num].play_name, BLACK, 24);
    /* 显示价格 */
    show_font(lcd, USER_CENTER_OUTPUT_PRICE_X, USER_CENTER_OUTPUT_PRICE_Y, 
                user_info.user_tickets[num].price, BLACK, 24);
    /* 显示开始时间 */
    show_font(lcd, USER_CENTER_OUTPUT_START_X, USER_CENTER_OUTPUT_START_Y , 
                user_info.user_tickets[num].time_start, BLACK, 24);
    /* 显示剧场标号 */
    char buf[20] = {0};
    sprintf(buf, "%d", user_info.user_tickets[num].theater_number);
    show_font(lcd, USER_CENTER_OUTPUT_ROOM_X, USER_CENTER_OUTPUT_ROOM_Y, 
                buf, BLACK, 24);
    /* 显示座位号 */
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d排 %d列", user_info.user_tickets[num].seat_number_x, 
                    user_info.user_tickets[num].seat_number_y);

    show_font(lcd, USER_CENTER_OUTPUT_SEAT_X, USER_CENTER_OUTPUT_SEAT_Y, 
                buf, BLACK, 24);
    return 0;
}

/********************************************
 * 
 *                  买票
 *              显示选票界面，选择剧片
 *      显示当前服务器存储的影片数量，以及价格
 *      
 * ******************************************/
int show_select_ticket(lcd_info_t* lcd, theater_info_t *theater_info, int tcp_socket)
{
    /* 创建临时账号，用于客户不用登录买票 */
    user_info_t temp;
    memset(&temp, 0, sizeof(temp));

chose_tick:     /* 选择影片 */

    /* 显示图片 */
    show_picture(lcd, IMAGE_SELECT_THEATER, 1, 0, 0);
    /* 显示内容 */
    show_theater_info(theater_info);
    while(1)
    {
        if(touch_flag != 1)
        {
            usleep(100 * 1000);
            continue;
        }
        touch_flag = 0;

        if(touch_x > BUTTON_USER_CENTER_NEXT_X && touch_x < BUTTON_USER_CENTER_NEXT_X + BUTTON_USER_CENTER_NEXT_WIDTH
            && touch_y > BUTTON_USER_CENTER_NEXT_Y && touch_y < BUTTON_USER_CENTER_NEXT_Y + BUTTON_USER_CENTER_NEXT_HEIGHT)
        {
            /* 用户按下下一张 */
            printf("next ticket\r\n");
            /* 像服务器发送获取剧场数据请求 */
            tcp_send(tcp_socket, CLIENT_SEL_NEXT, strlen(CLIENT_SEL_NEXT));
            /* 接收数据 */
            memset(theater_info, 0, sizeof(theater_info));
            if(tcp_recv(tcp_socket, theater_info, sizeof(theater_info_t)) < 0)
            {
                printf("error\r\n");
                touch_x = 0;
                touch_y = 0;
                continue;
            }
            // printf("play name = %s\r\n", theater_info->play_name);
           
            /* 更新界面 */
            show_theater_info(theater_info);
            touch_x = 0;
            touch_y = 0;
        }
        else if(BUTTON_USER_CENTER_PRE_X < touch_x && touch_x < BUTTON_USER_CENTER_PRE_X + BUTTON_USER_CENTER_PRE_WIDTH
            && BUTTON_USER_CENTER_PRE_Y < touch_y && touch_y < BUTTON_USER_CENTER_PRE_Y + BUTTON_USER_CENTER_PRE_HEIGHT)
        {
            /* 用户按下上一张 */
            printf("pre ticket\r\n");
            /* 像服务器发送获取剧场数据请求 */
            tcp_send(tcp_socket, CLIENT_SEL_PREV, sizeof(CLIENT_SEL_PREV));
            /* 接收数据 */
            memset(theater_info, 0, sizeof(theater_info));
            if(tcp_recv(tcp_socket, theater_info, sizeof(theater_info_t)) < 0)
            {
                printf("error\r\n");
                touch_x = 0;
                touch_y = 0;
                continue;
            }
            /* 更新界面 */
            show_theater_info(theater_info);
            touch_x = 0;
            touch_y = 0;
        }
        else if(touch_x > BUTTON_SELECT_CONFIRM_X && touch_x < BUTTON_SELECT_CONFIRM_X + BUTTON_SELECT_WIDTH
            && touch_y > BUTTON_SELECT_CONFIRM_Y && touch_y < BUTTON_SELECT_CONFIRM_Y + BUTTON_SELECT_HEIGHT)
        { 
            /* 用户按下确定 */
            printf("true\r\n");
            /* 将当前的剧场信息写入用户所用的票中 */
            write_theater_data_in_user(theater_info, &user_info);
            printf("theater info = %s\r\n", theater_info->play_name);
            printf("time = %s\r\n", theater_info->time_start);
            /* 服务器发送返回请求 */
            tcp_send(tcp_socket, CLIENT_EXIT, strlen(CLIENT_EXIT));
            touch_x = 0;
            touch_y = 0;
            break;      /* 退出当前循坏,进入选择时间界面 */
        }
        else if(touch_x > BUTTON_BACK_X && touch_x < BUTTON_BACK_X + BUTTON_BACK_WIDTH
            && touch_y > BUTTON_BACK_Y && touch_y < BUTTON_BACK_Y + BUTTON_BACK_HEIGHT)
        {
            /* 用户按下返回 */
            printf("back\r\n");
            /* 服务器发送返回请求 */
            tcp_send(tcp_socket, CLIENT_EXIT, strlen(CLIENT_EXIT));
            touch_x = 0;
            touch_y = 0;
            return BREAK_LAST;
        }
        else
        {
            printf("unkown\r\n");
            touch_x = 0;
            touch_y = 0;
            continue;
        }
    }

    /* 时间选择 */
    /* 根据经营时间和休息时间, 影片时长,生成每个时间点*/
    int array_len = 0;
    time_table_t* time_table = theater_time_table_init(theater_info, &array_len);
    if(time_table == NULL)
    {
        printf("time_table is NULL\r\n");
        return -1;
    }
      /* 显示时间界面 */
    show_picture(lcd, IMAGE_SELECT_TIME, 1, 0, 0);
    /* 选择最近的时间,显示4个时间段 */
    int index = show_time_table(time_table, 4, array_len);
    /* 捕获用户选择的时间 */
    while(1)
    {
        if(touch_flag != 1)
        {
            usleep(100 * 1000);
            continue;
        }
        touch_flag = 0;
        if(touch_x > TIME_SELECT_ONE_X && touch_x < TIME_SELECT_ONE_X + TIME_SELECT_WIDTH
            && touch_y > TIME_SELECT_ONE_Y && touch_y < TIME_SELECT_ONE_Y + TIME_SELECT_HEIGHT)
        {
            printf("time 1\r\n");
            /* 将时间一写入用户的票信息中 */
            strcpy(user_info.user_tickets[user_info.ticket_number].time_start, 
                                time_table[index].time_start);
            touch_x = 0;
            touch_y = 0;
            break;
        }
        else if(touch_x > TIME_SELECT_TWO_X && touch_x < TIME_SELECT_TWO_X + TIME_SELECT_WIDTH
            && touch_y > TIME_SELECT_TWO_Y && touch_y < TIME_SELECT_TWO_Y + TIME_SELECT_HEIGHT)
        {
            printf("time 2\r\n");
            strcpy(user_info.user_tickets[user_info.ticket_number].time_start, 
                            time_table[index + 1].time_start);
            touch_x = 0;
            touch_y = 0;
            break;  
        }
        else if(touch_x > TIME_SELECT_THREE_X && touch_x < TIME_SELECT_THREE_X + TIME_SELECT_WIDTH
            && touch_y > TIME_SELECT_THREE_Y && touch_y < TIME_SELECT_THREE_Y + TIME_SELECT_HEIGHT)
        {
            printf("time 3\r\n");

            strcpy(user_info.user_tickets[user_info.ticket_number].time_start, 
                            time_table[index + 2].time_start);
            touch_x = 0;
            touch_y = 0;
            break;
        }
        else if(touch_x > TIME_SELECT_FOUR_X && touch_x < TIME_SELECT_FOUR_X + TIME_SELECT_WIDTH
            && touch_y > TIME_SELECT_FOUR_Y && touch_y < TIME_SELECT_FOUR_Y + TIME_SELECT_HEIGHT)
        {
            printf("time 4\r\n");
            strcpy(user_info.user_tickets[user_info.ticket_number].time_start, 
                            time_table[index + 3].time_start);
            touch_x = 0;
            touch_y = 0;
            break;
        }
        else if(touch_x > BUTTON_BACK_X && touch_x < BUTTON_BACK_X + BUTTON_BACK_WIDTH
            && touch_y > BUTTON_BACK_Y && touch_y < BUTTON_BACK_Y + BUTTON_BACK_HEIGHT)
        {
            printf("back\r\n");
            touch_x = 0;
            touch_y = 0;
            goto chose_tick;
        }
        else
        {
            printf("unkown\r\n");
            touch_x = 0;
            touch_y = 0;
            continue;
        }
    }
    /* 选择座位号 */
    show_select_seat(lcd, tcp_socket, theater_info);
}


/********************************************
 * 
 *          显示剧场相关信息
 * 
 * ******************************************/
void show_theater_info(theater_info_t *theater_info)
{
    if(theater_info == NULL)
    {
        printf(" NULL\r\n");
        return;
    }
    /* 显示影片名字 */
    show_font(lcd, SELECT_OUTPUT_MOVE_X, SELECT_OUTPUT_MOVE_Y + 5, 
                theater_info->play_name, BLACK, 24);
   
    /* 显示价格 */
    show_font(lcd, SELECT_OUTPUT_PRICE_X, SELECT_OUTPUT_PRICE_Y + 5, 
                theater_info->price, BLACK, 24);
    /* 显示剧场编号 */
    char buf[10] ={0};
    sprintf(buf, "%2d号厅", theater_info->theater_number);
    show_font(lcd, SELECT_OUTPUT_NUMBER_X, SELECT_OUTPUT_NUMBER_Y, 
                buf, BLACK, 24);
    
    /* 再指定的位置显示图片 */
    /* 打开对应的图片 */
    // printf("==================\r\n");
    char file_path[128] = {0};
    sprintf(file_path, "%s/%s.jpg", 
                IMAGE_THEATER_PATH, theater_info->play_name);
    // printf("==================\r\n");
    show_picture(lcd , file_path, 1, SELECT_OUTPUT_IMAGE_X + 25,
             SELECT_OUTPUT_IMAGE_Y + 15);

}


/********************************************
 * 
 *            根据影片的播放时间和
 *      剧院的营业时长，计算每个影片的开始时间
 *              并将其汇总为时间表
 * *****************************************/
time_table_t * theater_time_table_init(theater_info_t * theater_info, int* array_len)
{
    /* 验证参数的有效性 */
    if(theater_info == NULL)
    {
        perror("theater_time_table_init: input argement error");
        return NULL;
    }
  
    /* 向时间表链表添加数据 */
    int count_all = theater_info->time_business * 60;  /* 分钟，20小时 */
    printf("count_all = %d\r\n", count_all);
    /* 剧片时长 + 15分钟的清洁 */
    int count_time = theater_info->time_play + 15;
    printf("count_time = %d\r\n", count_time);
    /* 计算持续 */
    int total_time = count_all / count_time;    /* 在这时间内总共播放次数 */
    printf("total_time = %d\r\n", total_time);

    /* 判断接收的时间是否有 : */
    if(strstr(theater_info->time_start, ":") == NULL)
    {
        perror("theater_time_table_init: time format error");
        
        /* 对时间添加: */
        sprintf(theater_info->time_start, "%s:%s", theater_info->time_start, "00");
    }
    printf("theater_info->time_start = %s\r\n", theater_info->time_start);
    /* 向时间表添加节点 */
    /*********************
     *  时间格式 小时：分钟
     * ******************/
    /* 分割开始时间 */
    int start_hour = 0, start_min = 0;
    /* 以:为分隔符 */
    char *hour = strtok(theater_info->time_start, ":");
    char* min = strtok(NULL, ":");
    /* 转换为整数 */
    start_hour = atoi(hour);
    start_min = atoi(min);
    printf("start hour = %d, start min = %d\r\n", start_hour, start_min);
    /* 生成时间表 */
    int time_table = start_hour * 60 + start_min;   /* 分钟 */
    char time_start[15] = {0};
    char time_end[15] = {0};
    /* 申请堆内存 */
    time_table_t *time_table_array = (time_table_t *)calloc(sizeof(time_table_t), total_time);
    if(time_table_array == NULL)
    {
        printf("calloc time_table error\r\n");
        return NULL;
    }
    time_table_t * temp = time_table_array;
    for(int i = 0; i < total_time; i ++)
    {
        if(time_table >= 24 * 60)
        {
            time_table -= 24 * 60;
        }
        /* 将开始时间写入时间表中 */
        memset(time_start, 0, sizeof(time_start));
        sprintf(time_start, "%02d:%02d", time_table / 60, time_table % 60);

        time_table += theater_info->time_play;
        if(time_table >= 24 * 60)
        {
            time_table -= 24 * 60;
        }
        /* 计算影片的开始时间和结算时间 */
        memset(time_end, 0, sizeof(time_end));
        sprintf(time_end, "%02d:%02d", time_table / 60, time_table % 60);

        time_table += 15;
        
        strcpy(temp[i].time_start, time_start);
        strcpy(temp[i].time_end, time_end);
        /* 添加数据 */
    }
    /* 打印时间 */
    // for(int i = 0; i < total_time; i ++)
    // {
    //     printf("time_start = %s, time_end = %s\r\n", time_table_array[i].time_start, time_table_array[i].time_end);
    // }
    *array_len = total_time;
    return time_table_array;
}

/*********************************
 * 
 *          显示时间段
 * 
 * *******************************/
int show_time_table(time_table_t *time_table, int count, int array_len)
{
    if(time_table == NULL)
    {
        printf("time_table is NULL\r\n");
        return -1;
    }
    /* 获取当前系统时间 */
    int local_time_hour = 0;
    char time_now[20] = {0};
    time_t t;
    struct tm *tm;
    time(&t);
    tm = localtime(&t);
    sprintf(time_now, "%02d", tm->tm_hour);
    local_time_hour = atoi(time_now);

    /* 遍历数组,找到比当前时间大的时间段 */
    int i = 0;
    for(i = 0; i < array_len; i ++)
    {
        /* 取出开始时间 */
        memset(time_now, 0, sizeof(time_now));
        strcpy(time_now, time_table[i].time_start);
        /* 分割出小时 */
        char *hour = strtok(time_now, ":");
        /* 转化成整数 */
        int start_hour = atoi(hour);
        if(start_hour > local_time_hour)
        {
            break;
        }
    }
    /* 显示开始时间 */
    /* 从i开始显示count个时间段 */
    int line = 0;
    for(int k = i; k < array_len && line < count; k++, line++)
    {
        /* 在指定位置显示开始时间 */
        char time_buf[30] = {0};
        sprintf(time_buf, "start:%s   end:%s", time_table[k].time_start, time_table[k].time_end);
        show_font(lcd, TIME_SELECT_ONE_X, TIME_SELECT_ONE_Y + 
                    line * (TIME_SELECT_HEIGHT + TIME_SELECT_PEIR_HEIGHT), 
                    time_buf, BLACK, 24);
    }
    return i;
}

/*********************************************
 * 
 *          将当前的剧场信息写入用户账号
 * 
 * ******************************************/
int write_theater_data_in_user(theater_info_t* theater_info, user_info_t *user_info)
{
    /* 将当前的剧场信息写入用户所用的票中 */
    strcpy(user_info->user_tickets[user_info->ticket_number].play_name,
                theater_info->play_name);
    strcpy(user_info->user_tickets[user_info->ticket_number].price,
                theater_info->price);
    char buf[10] = {0};
    sprintf(buf, "%d", theater_info->time_play);
    strcpy(user_info->user_tickets[user_info->ticket_number].time_play,
                buf);
    user_info->user_tickets[user_info->ticket_number].theater_number
            = theater_info->theater_number;
    return 0;
}


/***********************************************
 * 
 *              获取座位号
 * 
 * *********************************************/
int get_seat_number(int *seat_x, int* seat_y, int *bought_buf, int array_len)
{
    /* 验证参数的有效性 */
    int index = 0;
    int count = 0;
    if(seat_x == NULL || seat_y == NULL || bought_buf == NULL)
    {
        perror("select_seat: input argement error");
        return -1;
    }
    *seat_x = 0;
    *seat_y = 0;
    while(1)
    {
        /* 坐标捕获 */
        if(touch_flag != 1)
        {
            usleep(100* 1000);
            continue;
        }
        touch_flag = 0;
        printf("two touch = [%d] touch = [%d]\r\n", touch_x, touch_y);
        if(touch_x > SEAT_SELECT_1_X && touch_x < SEAT_SELECT_1_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_1_Y && touch_y < SEAT_SELECT_1_Y + SEAT_SELECT_HEIGHT)
        {
            index = 0;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            { 
                /* 判断坐标是否为当前坐标 */
                if(!(*seat_x == 1 && *seat_y == 1))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 清空座位号 */
                *seat_x = 0;
                *seat_y = 0;
                /* 显示不同座位图片 座位图标会变灰色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_1_X, SEAT_SELECT_1_Y);
                touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 1;
            *seat_y = 1;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_1_X, SEAT_SELECT_1_Y);
            touch_x = 0;
            touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_2_X && touch_x < SEAT_SELECT_2_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_2_Y && touch_y < SEAT_SELECT_2_Y + SEAT_SELECT_HEIGHT)
        {
            index = 1;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                continue;
            }
            if(count >= 2)
            {
                if (!(*seat_x == 2 && *seat_y == 1))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
               
                /* 清空座位表 */
                *seat_x = 0;
                *seat_y = 0;
                /* 显示不同座位图片 座位图标会变灰色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_2_X, SEAT_SELECT_2_Y);
                touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 2;
            *seat_y = 1;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_2_X, SEAT_SELECT_2_Y);
            touch_x = 0;
                    touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_3_X && touch_x < SEAT_SELECT_3_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_3_Y && touch_y < SEAT_SELECT_3_Y + SEAT_SELECT_HEIGHT)
        {
            index = 2;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 3 && *seat_y == 1))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 清空座位表 */
                *seat_x = 0;
                *seat_y = 0;
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_3_X, SEAT_SELECT_3_Y);
                touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 3;
            *seat_y = 1;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_3_X, SEAT_SELECT_3_Y);
            touch_x = 0;
            touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_4_X && touch_x < SEAT_SELECT_4_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_4_Y && touch_y < SEAT_SELECT_4_Y + SEAT_SELECT_HEIGHT)
        {
            index = 3;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                count = 0;
                if(!(*seat_x == 4 && *seat_y == 1))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_4_X, SEAT_SELECT_4_Y);
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            *seat_x = 4;
            *seat_y = 1;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_4_X, SEAT_SELECT_4_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_5_X && touch_x < SEAT_SELECT_5_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_5_Y && touch_y < SEAT_SELECT_5_Y + SEAT_SELECT_HEIGHT)
        {
            index = 4;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                count = 0;
                if(!(*seat_x == 5 && *seat_y == 1))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_5_X, SEAT_SELECT_5_Y);
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            *seat_x = 5;
            *seat_y = 1;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_5_X, SEAT_SELECT_5_Y);
            touch_x = 0;
                    touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_6_X && touch_x < SEAT_SELECT_6_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_6_Y && touch_y < SEAT_SELECT_6_Y + SEAT_SELECT_HEIGHT)
        {
            index = 5;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            
            if(count >= 2)
            {
                count = 0;
                if(!(*seat_x == 6 && *seat_y == 1))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_6_X, SEAT_SELECT_6_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 6;
            *seat_y = 1;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_6_X, SEAT_SELECT_6_Y);
            touch_x = 0;
                    touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_7_X && touch_x < SEAT_SELECT_7_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_7_Y && touch_y < SEAT_SELECT_7_Y + SEAT_SELECT_HEIGHT)
        {
            index = 6;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 7 && *seat_y == 1))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_7_X, SEAT_SELECT_7_Y);
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            *seat_x = 7;
            *seat_y = 1;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_7_X, SEAT_SELECT_7_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_8_X && touch_x < SEAT_SELECT_8_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_8_Y && touch_y < SEAT_SELECT_8_Y + SEAT_SELECT_HEIGHT)
        {
            index = 7;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 8 && *seat_y == 1))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_8_X, SEAT_SELECT_8_Y);
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            *seat_x = 8;
            *seat_y = 1;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_8_X, SEAT_SELECT_8_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_9_X && touch_x < SEAT_SELECT_9_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_9_Y && touch_y < SEAT_SELECT_9_Y + SEAT_SELECT_HEIGHT)
        {
            index = 8;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
            
            if(count >= 2)
            {
                if(!(*seat_x == 1 && *seat_y == 2))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_9_X, SEAT_SELECT_9_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 1;
            *seat_y = 2;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_9_X, SEAT_SELECT_9_Y);
            touch_x = 0;
            touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_10_X && touch_x < SEAT_SELECT_10_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_10_Y && touch_y < SEAT_SELECT_10_Y + SEAT_SELECT_HEIGHT)
        {
           index = 9;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                touch_x = 0;
                    touch_y = 0;
                continue;
            }
           
            if(count >= 2)
            {
                if(!(*seat_x == 2 && *seat_y == 2))
                {
                    printf("select wrong\r\n");
                    touch_x = 0;
                    touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_10_X, SEAT_SELECT_10_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 2;
            *seat_y = 2;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_10_X, SEAT_SELECT_10_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_11_X && touch_x < SEAT_SELECT_11_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_11_Y && touch_y < SEAT_SELECT_11_Y + SEAT_SELECT_HEIGHT)
        {
           index = 10;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 3 && *seat_y == 2))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_11_X, SEAT_SELECT_11_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 3;
            *seat_y = 2;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_11_X, SEAT_SELECT_11_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_12_X && touch_x < SEAT_SELECT_12_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_12_Y && touch_y < SEAT_SELECT_12_Y + SEAT_SELECT_HEIGHT)
        {
           index = 11;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            
            if(count >= 2)
            {
                if(!(*seat_x == 4 && *seat_y == 2))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_12_X, SEAT_SELECT_12_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 4;
            *seat_y = 2;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_12_X, SEAT_SELECT_12_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_13_X && touch_x < SEAT_SELECT_13_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_13_Y && touch_y < SEAT_SELECT_13_Y + SEAT_SELECT_HEIGHT)
        {
            index = 12;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 5 && *seat_y == 2))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_13_X, SEAT_SELECT_13_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 5;
            *seat_y = 2;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_13_X, SEAT_SELECT_13_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_14_X && touch_x < SEAT_SELECT_14_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_14_Y && touch_y < SEAT_SELECT_14_Y + SEAT_SELECT_HEIGHT)
        {
            index = 13;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 6 && *seat_y == 2))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_14_X, SEAT_SELECT_14_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 6;
            *seat_y = 2;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_14_X, SEAT_SELECT_14_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_15_X && touch_x < SEAT_SELECT_15_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_15_Y && touch_y < SEAT_SELECT_15_Y + SEAT_SELECT_HEIGHT)
        {
           index = 14;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            
            if(count >= 2)
            {
                if(!(*seat_x == 7 && *seat_y == 2))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_15_X, SEAT_SELECT_15_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 7;
            *seat_y = 2;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_15_X, SEAT_SELECT_15_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_16_X && touch_x < SEAT_SELECT_16_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_16_Y && touch_y < SEAT_SELECT_16_Y + SEAT_SELECT_HEIGHT)
        {
            index = 15;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 8 && *seat_y == 2))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_16_X, SEAT_SELECT_16_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 8;
            *seat_y = 2;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_16_X, SEAT_SELECT_16_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_17_X && touch_x < SEAT_SELECT_17_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_17_Y && touch_y < SEAT_SELECT_17_Y + SEAT_SELECT_HEIGHT)
        {
            index = 16;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 1 && *seat_y == 3))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_17_X, SEAT_SELECT_17_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 1;
            *seat_y = 3;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_17_X, SEAT_SELECT_17_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_18_X && touch_x < SEAT_SELECT_18_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_18_Y && touch_y < SEAT_SELECT_18_Y + SEAT_SELECT_HEIGHT)
        {
           index = 17;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 2 && *seat_y == 3))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_18_X, SEAT_SELECT_18_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 2;
            *seat_y = 3;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_18_X, SEAT_SELECT_18_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_19_X && touch_x < SEAT_SELECT_19_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_19_Y && touch_y < SEAT_SELECT_19_Y + SEAT_SELECT_HEIGHT)
        {
            index = 18;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 3 && *seat_y == 3))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_19_X, SEAT_SELECT_19_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 3;
            *seat_y = 3;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_19_X, SEAT_SELECT_19_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_20_X && touch_x < SEAT_SELECT_20_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_20_Y && touch_y < SEAT_SELECT_20_Y + SEAT_SELECT_HEIGHT)
        {
            index = 19;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            
            if(count >= 2)
            {
                if(!(*seat_x == 4 && *seat_y == 3))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_20_X, SEAT_SELECT_20_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 4;
            *seat_y = 3;
             /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_20_X, SEAT_SELECT_20_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_21_X && touch_x < SEAT_SELECT_21_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_21_Y && touch_y < SEAT_SELECT_21_Y + SEAT_SELECT_HEIGHT)
        {
            index = 20;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 5 && *seat_y == 3))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_21_X, SEAT_SELECT_21_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 5;
            *seat_y = 3;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_21_X, SEAT_SELECT_21_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_22_X && touch_x < SEAT_SELECT_22_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_22_Y && touch_y < SEAT_SELECT_22_Y + SEAT_SELECT_HEIGHT)
        {
            index = 21;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 6 && *seat_y == 3))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }   
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_22_X, SEAT_SELECT_22_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 6;
            *seat_y = 3;
             /* 显示不同座位图片 座位图标会变黄色 */
             show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_22_X, SEAT_SELECT_22_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_23_X && touch_x < SEAT_SELECT_23_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_23_Y && touch_y < SEAT_SELECT_23_Y + SEAT_SELECT_HEIGHT)
        {
            index = 22;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 7 && *seat_y == 3))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_23_X, SEAT_SELECT_23_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 7;
            *seat_y = 3;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_23_X, SEAT_SELECT_23_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_24_X && touch_x < SEAT_SELECT_24_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_24_Y && touch_y < SEAT_SELECT_24_Y + SEAT_SELECT_HEIGHT)
        {
            index = 23;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            
            if(count >= 2)
            {
                if(!(*seat_x == 8 && *seat_y == 3))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_24_X, SEAT_SELECT_24_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 8;
            *seat_y = 3;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_24_X, SEAT_SELECT_24_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_25_X && touch_x < SEAT_SELECT_25_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_25_Y && touch_y < SEAT_SELECT_25_Y + SEAT_SELECT_HEIGHT)
        {
            index = 24;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 1 && *seat_y == 4))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_25_X, SEAT_SELECT_25_Y);
                
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 1;
            *seat_y = 4;
             /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_25_X, SEAT_SELECT_25_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_26_X && touch_x < SEAT_SELECT_26_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_26_Y && touch_y < SEAT_SELECT_26_Y + SEAT_SELECT_HEIGHT)
        {
            index = 25;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            
            if(count >= 2)
            {
                if(!(*seat_x == 2 && *seat_y == 4))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_26_X, SEAT_SELECT_26_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 2;
            *seat_y = 4;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_26_X, SEAT_SELECT_26_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_27_X && touch_x < SEAT_SELECT_27_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_27_Y && touch_y < SEAT_SELECT_27_Y + SEAT_SELECT_HEIGHT)
        {
            index = 26;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 3 && *seat_y == 4))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_27_X, SEAT_SELECT_27_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 3;
            *seat_y = 4;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_27_X, SEAT_SELECT_27_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_28_X && touch_x < SEAT_SELECT_28_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_28_Y && touch_y < SEAT_SELECT_28_Y + SEAT_SELECT_HEIGHT)
        {
            index = 27;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 4 && *seat_y == 4))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_28_X, SEAT_SELECT_28_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 4;
            *seat_y = 4;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_28_X, SEAT_SELECT_28_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_29_X && touch_x < SEAT_SELECT_29_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_29_Y && touch_y < SEAT_SELECT_29_Y + SEAT_SELECT_HEIGHT)
        {
            index = 28;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 5 && *seat_y == 4))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_29_X, SEAT_SELECT_29_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 5;
            *seat_y = 4;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_29_X, SEAT_SELECT_29_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_30_X && touch_x < SEAT_SELECT_30_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_30_Y && touch_y < SEAT_SELECT_30_Y + SEAT_SELECT_HEIGHT)
        {
            index = 29;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            
            if(count >= 2)
            {
                if(!(*seat_x == 6 && *seat_y == 4))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_30_X, SEAT_SELECT_30_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 6;
            *seat_y = 4;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_30_X, SEAT_SELECT_30_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_31_X && touch_x < SEAT_SELECT_31_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_31_Y && touch_y < SEAT_SELECT_31_Y + SEAT_SELECT_HEIGHT)
        {
            index = 30;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2) 
            {
                if(!(*seat_x == 7 && *seat_y == 4))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_31_X, SEAT_SELECT_31_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 7;
            *seat_y = 4;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_31_X, SEAT_SELECT_31_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if(touch_x > SEAT_SELECT_32_X && touch_x < SEAT_SELECT_32_X + SEAT_SELECT_WIDTH
        && touch_y > SEAT_SELECT_32_Y && touch_y < SEAT_SELECT_32_Y + SEAT_SELECT_HEIGHT)
        {
            index = 31;
            count ++;
            if(array_match(bought_buf, array_len, index) == 1)
            {
                /* 座位已有人选 */
                printf("seat is bought\r\n");
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            if(count >= 2)
            {
                if(!(*seat_x == 8 && *seat_y == 4))
                {
                    printf("select wrong\r\n");
                     touch_x = 0;
                touch_y = 0;
                    continue;
                }
                count = 0;
                /* 用户取消当前选择 */
                printf("cancel select\r\n");
                /* 显示不同座位图片 座位图标会变黄色 */
                show_picture(lcd, SEAT_UNCHOSE, 1, SEAT_SELECT_32_X, SEAT_SELECT_32_Y);
                 touch_x = 0;
                touch_y = 0;
                continue;
            }
            *seat_x = 8;
            *seat_y = 4;
            /* 显示不同座位图片 座位图标会变黄色 */
            show_picture(lcd, SEAT_CHOSE, 1, SEAT_SELECT_32_X, SEAT_SELECT_32_Y);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
        else if (touch_x > SEAT_SELECT_CONFIRM_X && touch_x < SEAT_SELECT_CONFIRM_X + SEAT_SELECT_CONFIRM_WIDTH
        && touch_y > SEAT_SELECT_CONFIRM_Y && touch_y < SEAT_SELECT_CONFIRM_Y + SEAT_SELECT_CONFIRM_HEIGHT)
        {
            /* 确定按键 */
            printf("confirm select\r\n");
             touch_x = 0;
                touch_y = 0;
            return index;   /* 将用户选择的座位返回给服务器 */
        }
        else
        {
            usleep(100*1000);
             touch_x = 0;
                touch_y = 0;
            continue;
        }
    }
}


/****************************************
 * 
 *          显示选座界面
 * 
 * *************************************/
int show_select_seat(lcd_info_t *lcd, int accept_socket_fd, theater_info_t* theater_info)
{
    /* 请求服务器发送对应剧场的座位数据 */
    tcp_send(accept_socket_fd, CLIENT_GET_SEAT_DATA, strlen(CLIENT_GET_SEAT_DATA));
    printf("send data:%s\n", CLIENT_GET_SEAT_DATA);
    /* 发送用户选择的时间数据 */
    tcp_send(accept_socket_fd, user_info.user_tickets[user_info.ticket_number].time_start, 
                strlen(user_info.user_tickets[user_info.ticket_number].time_start));
    /* 接收座位表数据 */
    char recv_data_seat[128] = {0};
    if(tcp_recv(accept_socket_fd, recv_data_seat, sizeof(recv_data_seat)) < 0)
    {
        printf("recv data error\n");
        return -1;
    }
    printf("recv data:%s\n", recv_data_seat);       /* 接收32个0或1字符 */
    /* 显示选座图片 */
    show_picture(lcd, IMAGE_SELECT_SEAT, 1, 0, 0);
    /* 根据读取字符信息显示座位图标，红图标 */
    int bought_buf[32] = {0};
    int k = 0;
    int x = 0, y = 0;
    int i = 0;
    for(int height = 0; height < 4; height ++)
    {
        for(int width = 0; width < 8; width ++, i++)
        {
            
            if(recv_data_seat[i] == '1')
            {
                show_picture(lcd, SEAT_BUY, 1, SEAT_SELECT_1_X + 
                        width * (SEAT_SELECT_WIDTH + SEAT_SELECT_PEIR_WIDTH), 
                        SEAT_SELECT_1_Y + height * (SEAT_SELECT_HEIGHT + SEAT_SELECT_PEIR_HEIGHT));
                bought_buf[k++] = i;
            }
            
        }
    }
    /* 获取用户的触摸的座位图标 */
    int seat_x = 0, seat_y = 0;
    int index = get_seat_number(&seat_x, &seat_y, bought_buf, k);  /* 在这阻塞，等待用户按下位置 */
    printf("用户选择的座位是 %d, %d, index = %d\n", seat_x, seat_y, index);

    /* 对指定的下标进行赋值 为1 */
    recv_data_seat[index] = '1';
    /* 上传值至服务器存储 */
    /* 同步数据 */
    tcp_send(accept_socket_fd, recv_data_seat, strlen(recv_data_seat));
    printf("send data:%s\n", recv_data_seat);
    /* 将当前的座位号写入用户信息中 */
    user_info.user_tickets[user_info.ticket_number].seat_number_x = seat_x;
    user_info.user_tickets[user_info.ticket_number].seat_number_y = seat_y;
    
    
    return 0;
}

/************************************
 * 
 *          付款界面
 * 
 * *********************************/
int show_pay(lcd_info_t *lcd, int accept_socket_fd, theater_info_t* theater_info, 
            user_info_t* user_info)
{
    /* 显示付款界面 */
    show_picture(lcd, IMAGE_PAYMENT, 1, 0, 0);
    /* 显示基本信息 */
    show_font(lcd, PAYMENT_OUTPUT_MOVIE_X, PAYMENT_OUTPUT_MOVIE_Y,
            user_info->user_tickets[user_info->ticket_number].play_name, 
            BLACK, 24);
    char theater_number[20] = {0};
    sprintf(theater_number, "%d号厅", user_info->user_tickets[user_info->ticket_number].theater_number);
    show_font(lcd, PAYMENT_OUTPUT_THEATER_NO_X, PAYMENT_OUTPUT_THEATER_NO_Y,
            theater_number, 
            BLACK, 24);
    show_font(lcd, PAYMENT_OUTPUT_START_TIME_X, PAYMENT_OUTPUT_START_TIME_Y,
            user_info->user_tickets[user_info->ticket_number].time_start, 
            BLACK, 24);

    char seat_buf[20] = {0};
    sprintf(seat_buf, "%d行 %d排", 
        user_info->user_tickets[user_info->ticket_number].seat_number_x,
        user_info->user_tickets[user_info->ticket_number].seat_number_y);
    show_font(lcd, PAYMENT_OUTPUT_SEAT_X, PAYMENT_OUTPUT_SEAT_Y,
            seat_buf, 
            BLACK, 24);
    show_font(lcd, PAYMENT_OUTPUT_PRICE_X, PAYMENT_OUTPUT_PRICE_Y,
            user_info->user_tickets[user_info->ticket_number].price, 
            BLACK, 24);
    /* 坐标捕获 */
    while(1)
    {
        if(touch_flag != 1)
        {
            usleep(100 * 1000);
            continue;
        }
        touch_flag = 0;
        if(touch_x > PAYMENT_CONFIRM_X && touch_x < PAYMENT_CONFIRM_X + PAYMENT_CONFIRM_WIDTH
        && touch_y > PAYMENT_CONFIRM_Y && touch_y < PAYMENT_CONFIRM_Y + PAYMENT_CONFIRM_HEIGHT)
        {
            printf("confirm payment\r\n");
            /* 显示付款码 */
            show_picture(lcd, IMAGE_PAYMENT_QR_CODE, 1, 0, 0);
            /* 延时20s */
            // sleep(10);
            for(int i = 10; i >= 0; i--)
            {
                /* 在指定的位置显示数字 */
                char buf[3] = {0};
                sprintf(buf, "%d", i);
                show_font_number(lcd,PAYMENT_COUNTDOWN_X, 
                    PAYMENT_COUNTDOWN_Y, buf,32,BLACK,32);
                sleep(1);
            }
            /* 查看是否已经登录 */
            if(is_login == 1)
            {
                /* 向服务器发送同步用户数据请求 */
                user_info->ticket_number++;
                tcp_send(accept_socket_fd, CLIENT_SYS_USER_DATA, strlen(CLIENT_SYS_USER_DATA));
                /* 发送当前的用户信息 */
                send(accept_socket_fd, user_info, sizeof(user_info_t), 0);
                /* 清空用户信息 */
                memset(&user_info, 0, sizeof(user_info));
                /* 清空剧场数据 */
                memset(&theater_info, 0, sizeof(theater_info));
            }
            else
            {
                /* 清空用户信息 */
                memset(&user_info, 0, sizeof(user_info));
                /* 清空剧场数据 */
                memset(&theater_info, 0, sizeof(theater_info));
            }
            /* 回到待机界面 */
            return 0;
        }
        else
        {
            usleep(100 * 1000);
            continue;
        }
    }
}