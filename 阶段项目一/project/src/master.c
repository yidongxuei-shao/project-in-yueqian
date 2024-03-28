#include "master.h"

/* Copyright 12/8/2023 Szm */

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
int show_master(Hnode_t* Pmanage, LcdInfo_t* lcd_info, int ts_fd)
{
    /* 一个界面显示一个用户信息
        显示用户的账号，密码、余额、创建日期
    */
   /*
        功能：修改用户密码
                用户余额
                两个按键，上一用户，下一用户
   */

    /* 显示商家登录界面 */      /* 图片30 */
    char master_account[] = MASTER_ACCOUT;      /* 商家账号 */
    char master_password[] = MASTER_PASSWORD;   /* 商家密码 */

    char buf_acc[10] = {0};         /* 输入账号 */
    char buf_password[10] = {0};    /* 输入的密码 */
    int x, y;
    int flag = 0;
    // read_JPEG_file(0,0, PIC_MASTER_LOGIN, lcd_info);

intput_account_again:   /* 重新输入账号 */
    flag = show_input(ts_fd, lcd_info, PIC_MASTER_LOGIN, buf_acc,
                         strlen(MASTER_ACCOUT), NUMBER_POS_STA_X, NUMBER_POS_STA_Y);
    if(flag == -1)
        return -1;    /* 返回上一层界面 */
    /* 判断输入的账号秘密 */
    /* 判断账号是否正确 */
    if(strcmp(buf_acc, MASTER_ACCOUT) != 0)
    {
        /* 输入的账号错误 */
        read_JPEG_file(150,100, PIC_MASTER_LOGIN_ERROR, lcd_info);
        /* 设置坐标值 */
        set_true_or_false_key(150, 100);
        /* 获取用户坐标 */
get_ts_val_agein:   /* 重新获取坐标值 */

        TS_GetVal(ts_fd, &x, &y);
        /* 确定 */
        if(x > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)
        {
            /* 重新输入 */
            goto intput_account_again;
        }
        else if(x > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end)
        {
            /* 退出函数，返回上一层 */
            return -1;
        }
        else
        {
            /* 重新获取坐标值 */
            goto get_ts_val_agein;
        }
    }
    /* 输出的登陆成功 */
    /* 提示商家输入密码 */
    char password_buf[10] = {0};
intput_password_again:
    flag = show_input(ts_fd, lcd_info, PIC_MASTER_PASSWORD, password_buf,
                         strlen(MASTER_PASSWORD), NUMBER_POS_STA_X, NUMBER_POS_STA_Y);
    if(flag == -1)
        return -1;    /* 返回上一层界面 */
    /* 判断输入的账号秘密 */
    /* 判断账号是否正确 */
    if(strcmp(password_buf, MASTER_PASSWORD) != 0)
    {
        /* 输入的账号错误 */
        read_JPEG_file(150,100, PIC_MASTER_PASSWORD_ERROR, lcd_info);
        /* 设置坐标值 */
        set_true_or_false_key(150, 100);
        /* 获取用户坐标 */
get_ts_val_agein_2:   /* 重新获取坐标值 */

        TS_GetVal(ts_fd, &x, &y);
        /* 确定 */
        if(x > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)
        {
            /* 重新输入 */
            goto intput_account_again;
        }
        else if(x > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end)
        {
            /* 退出函数，返回上一层 */
            return -1;
        }
        else
        {
            /* 重新获取坐标值 */
            goto get_ts_val_agein_2;
        }
    }
    /* 显示登录成功界面 */
    read_JPEG_file(150,100, PIC_MASTER_LOGIN_SUC, lcd_info);
get_login_suc:
    TS_GetVal(ts_fd, &x, &y);
    if(x > 150 && x < 150 + 500 && y > 100 && y < 100 + 200)
    {
         /* 遍历链表，依次遍历用户信息 */
        Node_t* p = Pmanage->Head;
        while(p)
        {
            /* 使用字库在lcd屏上显示用户数据 */
            show_user_mess(200,100,40,p->data,lcd_info, SHOW_USER_MSG_IN_MASTER
                , getColor(0, 255, 255,255), getColor(0,0,0,0));            /* 白底黑字 */
    get_val_again:
            TS_GetVal(ts_fd, &x, &y);
            /* 左下角， 上一个 */
            if(x > 0 && x < 100 && y > 0 && y < 480)
            {
                p = p->prev;
                if(p == NULL)
                {
                    /* 说明到了第一页的前一页 */
                    read_JPEG_file(150,100, PIC_IS_FIRST_INFO, lcd_info);
                    TS_GetVal(ts_fd, &x, &y);
                    return -1;      /* 返回上一层 */
                }
            }
            /* 右下角， 下一个 */
            else if(x > 800 - 100 && x < 800 && y > 480 - 100 && y < 480)
            {
                p = p->next;
                if(p == NULL)
                {
                    /* 说明到了第一页的前一页 */
                    read_JPEG_file(150,100, PIC_IS_LAST_INFO, lcd_info);
                    TS_GetVal(ts_fd, &x, &y);
                    return -1;       /* 返回上一层 */
                }
            }
            /* 无效值 */
            else
            {
                goto get_val_again;
            }
        }
    }
    else
    {
        /* 重新获取 */
        goto get_login_suc;
    }
}
