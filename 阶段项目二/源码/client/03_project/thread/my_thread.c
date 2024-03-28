#include "my_thread.h"

/**************************************
 * 
 *          临界资源
 * 
 * ***********************************/
pthread_mutex_t touch_ret_val_mutex =  PTHREAD_MUTEX_INITIALIZER;    /* 查看touch_ret_val的临界资源 */        


/**************************************
 * 
 *        触摸屏线程
 * 
 * ***********************************/
volatile int touch_x, touch_y;
volatile int touch_flag = 0;
void* touch_thread(void* arg)
{
    /* 初始化触摸屏 */
    int touch_fd = touch_init();
    if(touch_fd < 0)
    {
        printf("touch init failed\n");
        return NULL;
    }
    /* 初始化touch返回值互斥锁 */
    pthread_mutex_init(&touch_ret_val_mutex, NULL);
    while(1)
    {
        touch_read_xy(touch_fd, &touch_x, &touch_y);
        /* 上锁 */
        // pthread_mutex_lock(&touch_ret_val_mutex);
        touch_flag = 1;
        /* 解锁 */
        // pthread_mutex_unlock(&touch_ret_val_mutex);
    }
    return NULL;
}

/*************************************
 * 
 *        播放开机动画线程
 * 
 * **********************************/

void* thread_start_animation(void* arg)
{
    lcd_info_t * lcd_info = *(lcd_info_t **)arg;
    pthread_detach(pthread_self());
    while(1)
    {
        show_GIF(lcd_info, GIF_PATH, 0, 0, 20);
        sleep(1);
    }
    
    return NULL;
}

/**********************************************
 * 
 *            待机界面线程
 *      创建线程，每隔60s，在界面刷新时间
 *               一个小时，获取天气数据，在界面显示
 * *******************************************/
/* 信号响应接口 */
pthread_t timer_thread;     /* 计时线程TID */
void signal_end_counter(int sig)
{
    /* 结束计时器线程 */
    pthread_cancel(timer_thread);
    /* 结束当前线程 */
    pthread_exit(NULL);
}
/*************************
 *        计时子线程
 * **********************/
void* task_timer_thread(void* arg)
{

    lcd_info_t * lcd_info = (lcd_info_t *)arg;
    pthread_detach(pthread_self());
    /* 获取当前时间 */
    time_t t = time(NULL);
    struct tm * now = localtime(&t);
    show_time(lcd_info, 547 + 20, 80);
    /* 输出天气信息 */
    // printf("temp = %d, text = %s\r\n", weather_data.temp, weather_data.weather);
    /* 显示在lcd屏上 */
    show_weather(lcd_info, &weather_data, 520, 240);
    /* 获取当前剩余的秒数 */
    int clock_sec = now->tm_sec;
    // printf("clock_sec = %d\r\n", clock_sec);
    /* 显示时间 */
    show_time(lcd_info, 547 + 20, 80);
    /* 延时剩余的秒数 */
    sleep(60 - clock_sec);
    int count = 0;
    while(1)
    {
        /* 以分钟为单位显示时间 */
        /* 显示当前时间 */
        show_time(lcd_info, 547 + 20, 80);
        sleep(20);
        sleep(20);
        sleep(20);
        count ++;
        if(count == 60)
        {
            count = 0;
            /* 获取天气数据 */
            tcp_get_weather_data(tcp_socket_fd, &weather_data);
            /* 显示在lcd屏上 */
            show_weather(lcd_info, &weather_data, 547, 240);
        }
    }
    
    return NULL;
}
void* thread_start_video(void* arg)
{
    lcd_info_t * lcd_info = (lcd_info_t *)arg;
    /* 注册用户信号 */
    signal(SIGUSR1, signal_end_counter);

    /* 显示待机图片 */
    show_picture(lcd_info, IMAGE_WAITTING, 1, 0, 0);
    pthread_detach(pthread_self());

    /* 创建计时线程 */
    /* 每个60s获取一次时间，并显示在屏幕上 */
    pthread_create(&timer_thread, NULL, task_timer_thread, (void *)lcd_info);

    /* 获取视频广告的名称 */
    int video_num = 0;
    char** video_name = NULL;
    dir_file(AUDIO_PATH, ".avi", &video_num, &video_name);
    int i = 0;
    while(1)
    {
        for(i = 0; i < video_num; i++)
        {
            mplayer_play(VIDEO_PATH, video_name[i]);
            sleep(4 * 60);
            /* 关闭mplayer */
            mplayer_stop();
        }
    }
    return NULL;
}

/*******************************************
 * 
 *              用户按键线程
 * 
 * *****************************************/
int touch_ret_val = 0;
int user_ret_val = 0;
void* task_user_key_main(void* arg)
{
    lcd_info_t * lcd_info = (lcd_info_t *)arg;
    int count = 0;
    pthread_detach(pthread_self());
    sleep(1);       /* 等待界面加载完毕 */
    while(1)
    {
        #if 1
        /* 上锁 */
        pthread_mutex_lock(&touch_ret_val_mutex);
        if(touch_flag == 1)
        {
            touch_flag = 0;
            /* 解锁 */
            pthread_mutex_unlock(&touch_ret_val_mutex);
            count++;
            /* 取票 */
            #if 1
            if(touch_x > BUTTON_MAIN_GET_X && touch_x < BUTTON_MAIN_GET_X + BUTTON_MAIN_WIDTH
                && touch_y > BUTTON_MAIN_GET_Y && touch_y < BUTTON_MAIN_GET_Y + BUTTON_MAIN_HEIGHT)
            {
                printf("get ticket\r\n");
                /* 图片替换为背景色为蓝色 */
                show_picture(lcd_info, IMAGE_BUTTON_MAIN_GET, 1, 
                                BUTTON_MAIN_GET_X, BUTTON_MAIN_GET_Y);
                
                /* 松开按键 */
                show_picture(lcd_info, IMAGE_BUTTON_MAIN_GET_RELEASE, 1, 
                                BUTTON_MAIN_GET_X, BUTTON_MAIN_GET_Y);
                /* 向用户操作线程发送信号，结束运行 */
                usleep(100 * 1000);
                pthread_kill(thread_user_operation, SIGUSR1);

                // user_ret_val = GET_TICKS;
                // return (void*)&user_ret_val;
                user_ret_val = GET_TICKS;
                pthread_exit(NULL); 
            }
            #endif
            else if(touch_x > BUTTON_MAIN_BUY_X && touch_x < BUTTON_MAIN_BUY_X + BUTTON_MAIN_WIDTH
                && touch_y > BUTTON_MAIN_BUY_Y && touch_y < BUTTON_MAIN_BUY_Y + BUTTON_MAIN_HEIGHT)
            {
                printf("buy ticket\r\n");
                /* 向用户操作线程发送信号，结束运行 */
                pthread_kill(thread_user_operation, SIGUSR1);

                user_ret_val = BUY_TICKS;
                pthread_exit(NULL); 
            }
            else if(touch_x > BUTTON_MAIN_SHOPPING_X && touch_x < BUTTON_MAIN_SHOPPING_X + BUTTON_MAIN_WIDTH
                && touch_y > BUTTON_MAIN_SHOPPING_Y && touch_y < BUTTON_MAIN_SHOPPING_Y + BUTTON_MAIN_HEIGHT)
            {
                printf("shopping\r\n");

                /* 向用户操作线程发送信号，结束运行 */
                pthread_kill(thread_user_operation, SIGUSR1);

                user_ret_val = SHOPPING;
                pthread_exit(NULL); 
            }
           
        }
        else
        {
            if(count > 0)
            {
                count --;
                /* 图片替换为背景色为白色 */
                // show_picture(lcd_info, IMAGE_BUTTON_MAIN_GET_RELEASE, 1,
                //                 BUTTON_MAIN_GET_X, BUTTON_MAIN_GET_Y);
            }
        }
        /* 解锁 */
        pthread_mutex_unlock(&touch_ret_val_mutex);
        usleep(100 * 1000);    /* 100ms */ 
        #endif
    }
    return NULL;
}

