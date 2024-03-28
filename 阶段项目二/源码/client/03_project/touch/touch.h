#ifndef _TOUCH_H_
#define _TOUCH_H_

/*********************************************
 * 
 *              头文件
 * 
 * *******************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/types.h>
#include <fcntl.h>
/*********************************************
 * 
 *               宏定义         
 * 
 * ******************************************/
#define TOUCH_PATH "/dev/input/event0"



/*********************************************
 * 
 *               函数声明         
 * 
 * ******************************************/
int touch_init(void);
int touch_read_xy(int fd, volatile int* x, volatile int* y);
void touch_detroy(int touch_fd);

#endif // !_TOUCH