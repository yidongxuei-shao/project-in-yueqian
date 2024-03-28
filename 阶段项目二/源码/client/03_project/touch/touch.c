#include "touch.h"

/****************************************************
 * 函数名称: touch_init
 * 功能描述: 初始化触摸屏
 * 输入参数: 无
 * 返回值：  成功返回触摸屏的文件描述符，失败返回-1
 * 作者：SZM
 * 日期：2023年12月25日
 * **************************************************/
int touch_init(void)
{
    int fd = 0;
    fd = open(TOUCH_PATH, O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    return fd;
}

/****************************************************
 * 函数名称: touch_read_xy
 * 功能描述: 读取触摸屏的坐标
 * 参数:    @fd：触摸屏的文件描述符
 *          @x：x坐标
 *          @y：y坐标
 * 返回值：  成功返回0，失败返回-1
 * 作者：SZM
 * 日期：2023年12月25日
 * **************************************************/
int touch_read_xy(int fd, volatile int* x, volatile int* y)
{
    struct input_event event;
    int ret = 0, count = 0;
    int tempX = 0, tempY = 0;
    while(1)
    {
        ret = read(fd, &event, sizeof(event));
        if (ret < 0)
        {
            perror("read touch");
            return -1;
        }
        if (event.type == EV_ABS && event.code == ABS_X )
        {
            tempX = event.value * 800 / 1024;
            count++;   
        }
        if (event.type == EV_ABS && event.code == ABS_Y )
        {
            tempY = event.value * 480 / 600;
            count++;
        }
        if(count >= 2)
        {
            *x = tempX;
            *y = tempY;
            count = 0;
            break;
        }
    }
    
    return 0;
}
/***********************************************
 * 函数名称: touch_detroy
 * 功能描述: 关闭触摸屏的文件
 * 参数:    @touch_fd：触摸屏的文件描述符
 * 返回值：  无
 * 作者：SZM
 * 日期：2023年12月25日
 * *********************************************/
void touch_detroy(int touch_fd)
{
    close(touch_fd);
}