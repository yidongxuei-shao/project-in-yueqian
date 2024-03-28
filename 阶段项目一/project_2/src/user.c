
/*
    用户界面显示：
        1、显示当前登录账号的详细信息
        2、可以选择充值、充电
*/
#include "user.h"

int user_interface_show(DataType_t* user_info, LcdInfo_t* lcd_info, int ts_fd)
{
    int x, y;
    /* 当用户登录成功后显示，当前用户界面 */
    read_JPEG_file(0,0,PIC_USER_SHOW, lcd_info);
    /* 该界面有
        1、用户信息
        2、充值
        3、充电
    */
    /* 设置用户界面的图片坐标 */
    set_user_interface_key();
    /* 获取用户触摸坐标 */

get_ts_val_again:
    TS_GetVal(ts_fd, &x,&y);
    if(x > user_msg.x_start && x < user_msg.x_end && y > user_msg.y_start && y < user_msg.y_end)
    {
        /* 跳转到显示用户信息界面 */
        return 1;
    }
    else if(x > charge.x_start && x < charge.x_end && y > charge.y_start && y < charge.y_end)
    {
        /* 跳转到充电界面 */
        return 2;
    }
    else if(x > recharge.x_start && x < recharge.x_end && y > recharge.y_start && y < recharge.y_end)
    {
        /* 跳转到充值界面 */
        return 3;
    }
    else if(x > back.x_start && x < back.x_end && y > back.y_start && y < back.y_end)
    {
        /* 跳转到上一界面 */
        return -1;
    }
    else
    {
        goto get_ts_val_again;
    }
}

/* 用户充值界面 */
int user_recharge_balance(LcdInfo_t* lcd_info, int ts_fd, DataType_t* cur_user, Hnode_t* Pmanage)
{
    
    char buf[30] = {0};
    /* 设置键位坐标 */
    set_postion_keypad();
    /* 呼出输入键盘 */
    int flag =  show_input(ts_fd,lcd_info, PIC_RECHAGER, buf, 
                        CHARGE_MAX, 270, 92);
    if(flag == -1)
        return -1;

    printf("buf = %s\r\n", buf);
    unsigned int temp = 0;
    /* 写入用户账号中 */
    /* 将输入的字符串转化成整形 */
    temp = atoi(buf);
    printf("temp = %d\r\n", temp);

    cur_user->balance += temp;
    printf("cur_user->balance = %d\r\n", cur_user->balance);
    printf("cur_user->account = %s\r\n", cur_user->account);
    /* 当用户充值成功时，将充值的数据写入文件中，以便下次使用 */
    write_file_balance(Pmanage, cur_user->account, cur_user->balance);

    
    /* 显示充值成功 */
    read_JPEG_file(150, 100, PIC_RECHAGER_SUC, lcd_info);

     /* 显示充值成功 */
        read_JPEG_file(150, 100, PIC_RECHAGER_SUC, lcd_info);

    int x, y;
show_charge_suucc_error:    /* 当用户点击其他地方时，重新获取坐标判断 */
        TS_GetVal(ts_fd, &x, &y);
        /* 150 是 图片x轴偏移值， 100 是图片y轴的偏移值 */
        if((x > 150 + 190 && x < 150 + 290 && y > 100 + 100 && y < 100 + 145)
                || (x > 150 + 337 && x < 150 + 434 && y > 100 + 98 && y < 100 + 145))     /* 确定 和 取消 */
        {
            printf("return pay interface!\r\n");
          return 0;
        }
        else
        {
            /* 用户点击的位置不是，确定或者取消，返回重新获取坐标 */
            goto show_charge_suucc_error;
        }

    return 0;
}


/* 用户信息查看 */
#if 1

/*
    函数名称：show_user_mess
    函数功能：   通过字库函数的调用，在lcd显示一个用户的具体信息
    函数参数：
                @x：整体字体的x轴偏移值
                @y：整体字体的y轴偏移值
                @font_size：字体的大小
                @data：显示的用户数据节点
                @lcd：记录lcd参数结构体指针变量
    返回值：    None
    函数作者：Szm
    创建日期：10/9/2023
    函数版本：V1.0
*/
void show_user_mess(int x, int y, int font_size, DataType_t data, 
                        LcdInfo_t* lcd, int mode, unsigned int bitmap_color, unsigned int font_color)
{
    /* 打开字体 */
	font *my_font = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	  
	/* 字体大小的设置 */
	fontSetSize(my_font, font_size);

    /* 创建画板 */
    bitmap* bite_mp = createBitmapWithInit(lcd->width, lcd->height, 
                    lcd->bpp, bitmap_color);            /* 白板 */

    /* 将用户数据的写入点阵图 */
    char buf[100] = {0};
    char user_account[] = {"账号："};
    char user_password[] = {"密码："};
    char user_blance[] = {"余额："};
    char user_create_time[] = {"创建时间："};
    char uer_balance_time[] = {"充值时间："};   

    /* 将字体写到点阵图上 */
    fontPrint(my_font, bite_mp, 0 ,0, "返回", font_color,0);

    fontPrint(my_font, bite_mp, 350 ,20, "用户信息", font_color,0);

    /* 显示账号 */
    fontPrint(my_font, bite_mp, x ,y, user_account, font_color,0);
    fontPrint(my_font, bite_mp, x + font_size * 3 ,y, data.account, font_color,0);

    /* 显示密码 */
    fontPrint(my_font, bite_mp, x ,y + font_size, user_password, font_color,0);
    fontPrint(my_font, bite_mp, x + font_size * 3 ,y + font_size, data.password, font_color,0);

    sprintf(buf, "%4d", data.balance);
    /* 显示余额 */
    fontPrint(my_font, bite_mp, x, y + font_size * 2, user_blance, font_color,0);
    fontPrint(my_font, bite_mp, x + font_size * 3 ,y + font_size* 2, buf, font_color,0);

    /* 显示创建日期 */
    fontPrint(my_font, bite_mp, x, y + font_size * 3, 
                        user_create_time, font_color,0);
    fontPrint(my_font, bite_mp, x + 20 + font_size * 3 ,y + font_size* 3, 
                        data.time_account_create, font_color,0);

    /* 显示充值时间 */
    fontPrint(my_font, bite_mp, x, y + font_size * 4, 
                        uer_balance_time, font_color,0);
    fontPrint(my_font, bite_mp, x + 20 + font_size * 3 ,y + font_size* 4, 
                        data.time_charge_balance, font_color,0);
                    

    if(mode == SHOW_USER_MSG_IN_MASTER)
    {
         /* 上一个 */
        char priv[] = {"上一个"};
        fontPrint(my_font, bite_mp, 0 ,480 - y + 15, priv, font_color,0);
        /* 下一个 */
        char next[] = {"下一个"};
        fontPrint(my_font, bite_mp, 700 ,480 - y + 15, next, font_color,0);
    }
   
    /* 将数据向lcd屏进行写入 */
    /* 
        第二参数是x轴的偏移值
        第三参数是y轴的偏移值
    */
    show_font_to_lcd(lcd->mp, 0, 0, bite_mp);


    //关闭字体，关闭画板
	fontUnload(my_font);
	destroyBitmap(bite_mp);
    
}

#endif

