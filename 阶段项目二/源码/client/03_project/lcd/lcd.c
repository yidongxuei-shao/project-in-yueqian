#include "lcd.h"
/**************************************************
 * 函数名称：lcd_init
 * 功能说明：初始化lcd
 * 参数：none
 * 返回值：lcd_info_t指针，失败返回NULL
 * 函数作者：SZM
 * 时间：2023年12月25日
 * ***********************************************/
lcd_info_t* lcd_init(void)
{
    /* 申请堆内存 */
    lcd_info_t *lcd_info = (lcd_info_t *)calloc(1, sizeof(lcd_info_t));
    if(lcd_info == NULL)
    {
        perror("can not calloc");
        return NULL;
    }

    /* 打开文件 */
    lcd_info->lcd_fd = open(LCD_PATH, O_RDWR);
    if(lcd_info->lcd_fd < 0)
    {
        perror("can not open lcd\r\n");
        return NULL;
    }
    // printf("************************\r\n");
    /* 获取屏幕的高和宽 */
    struct fb_var_screeninfo screen_info;
    ioctl(lcd_info->lcd_fd, FBIOGET_VSCREENINFO, &screen_info);

    lcd_info->height = screen_info.yres;
    lcd_info->widch = screen_info.xres;
    lcd_info->colour_deep = screen_info.bits_per_pixel / 8;

    printf("witch = %d, height = %d , deep = %d\r\n", 
                                    lcd_info->widch, lcd_info->height, lcd_info->colour_deep);
    /* 地址映射 */
    lcd_info->lcd_mp = mmap(
                        NULL,                           /* 内存分配地址 */
                        lcd_info->height * lcd_info->widch 
                        * lcd_info->colour_deep,     /* 申请内存的长度 */
                        PROT_READ | PROT_WRITE,         /* 内存的权限 */
                        MAP_SHARED,                     /* 共享内存 */
                        lcd_info->lcd_fd,               /* 文件句柄 */
                        0                               /* 偏移字节 */
                    );

    return lcd_info;
}

/* 销毁内存，关闭文件 */

/************************************************
 * 函数名称：lcd_destroy
 * 功能说明：取消映射，释放内存，关闭文件
 * 参数：@lcd_info: lcd信息结构体
 * 返回值：none
 * 函数作者：SZM
 * 时间：2023年12月25日
*************************************************/
void lcd_destroy(lcd_info_t * lcd_info)
{
    close(lcd_info->lcd_fd);
    munmap(lcd_info->lcd_mp,
                lcd_info->widch * lcd_info->height * lcd_info->colour_deep);
    free(lcd_info);
}