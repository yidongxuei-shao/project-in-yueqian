#ifndef MASTER_H
#define MASTER_H

/* Copyright 12/8/2023 Szm */

#include "double_list.h"
#include "lcd.h"
#include "ts.h"
#include "user.h"
#include "main.h"


/*
    函数名称：show_master
    函数功能：   从链表中读取用户数据，通过字库输出到lcd上
                管理员可以通过点击触摸屏显示
                的上一页和下一页浏览用户具体信息，当到用户信息节点的末尾或开头
                在点击下一页或上一页，会显示提示框，然后回到上一层界面
    函数参数：
                @Pmanage：管理链表的头节点
                @lcd_info：记录lcd参数结构体指针变量
                @ts_fd：    触摸屏的文件描述符
    返回值：    None
    函数作者：Szm
    创建日期：10/9/2023
    函数版本：V1.0
*/
int show_master(Hnode_t* Pmanage, LcdInfo_t* lcd_info, int ts_fd);


#endif // !1