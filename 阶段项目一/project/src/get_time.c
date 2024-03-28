#include "get_time.h"

/*  Copyright 12/8/2023 Szm
    获取系统当前时间，返回时间字符串 
*/
/*
    函数名称：get_time
    函数功能：获取当前的系统时间，返回get_time_t结构体指针
    函数参数：None
    返回值：    成功返回get_time_t结构体指针，失败返回NULL
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
get_time_t* get_time(void)
{
    printf("entry time\r\n");

    /* 获取系统时间 */
    time_t get_time = time(NULL);   

    char buf[30] = {0};
    /* 将获取到的时间转成字符串，并将时间字符串前24个拷贝到临时变量中
    为了，去除时间字符串中的换行符（'\n'） */
    strncpy(buf, ctime(&get_time), TIME_LONG - 1);
    // char * buf_1 = NULL;

    /* 申请堆内存 */
    get_time_t *time_data =(get_time_t *)calloc(sizeof(get_time_t), 1);
    if(time_data == NULL)
    {
        perror("calloc for time failed!\r\n");
        return NULL;
    }
    #if 0
    // printf("%s", buf);
    strcpy(time_data->time_all, buf);

    /* 分割字符串，得到 时：分：秒 */
    strtok(buf, " ");
    for(int i = 0; i < 3; i++)
    { 
       buf = strtok(NULL, " ");
    }
    // printf("%s\r\n", buf);

    time_data->hour = atoi(strtok(buf, ":"));       // 时
        //  printf("%s\r\n", time_data->hour); 

    buf = strtok(NULL, " ");    
    //  printf("%s\r\n", buf);     //剩下 时：分

    time_data->min = atoi(strtok(buf, ":"));     //分
    // printf("%s\r\n", time_data->min);

    time_data->sec = atoi(strtok(NULL, " "));  //秒
    // printf("%s\r\n", time_data->sec);
    #endif

    /* 分割字符串 */
    get_time_devi(buf, time_data);

    return time_data;
}

/* 从时间字符串分割出，时分秒，存储在get_time_t结构体中 */
/*
    函数名称：get_time_devi
    函数功能：从时间字符串分割出，时分秒，存储在get_time_t结构体中
    函数参数：
                @time：时间字符串
                @time_data：分割结束后的存储指针
    返回值：    None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
void get_time_devi(char* time, get_time_t *time_data)
{
    /* 将字符串放入get_time结构体中，备份 */
    strcpy(time_data->time_all, time);

    /* 分割字符串，得到 时：分：秒 */
    strtok(time, " ");
    for(int i = 0; i < 3; i++)
    { 
       time = strtok(NULL, " ");
    }

    time_data->hour = atoi(strtok(time, ":"));       // 时
       
    time = strtok(NULL, " ");    

    time_data->min = atoi(strtok(time, ":"));     //分
  
    time_data->sec = atoi(strtok(NULL, " "));  //秒
   
}

/*
    函数名称：show_time
    函数功能：打印输入time内的数据
    函数参数：
                @time：存储时间结构体指针
    返回值：    None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
void show_time(get_time_t *time)
{
    printf("time all = %s\r\n", time->time_all);
    printf("hour = %d\r\n", time->hour);
    printf("min = %d\r\n", time->min);
    printf("sec = %d\r\n", time->sec);
}


