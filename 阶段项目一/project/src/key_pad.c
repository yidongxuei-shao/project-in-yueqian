#include "key_pad.h"
/* Copyright 12/8/2023 Szm */

/***************全局变量*******************/
/* 定位数字键盘的坐标 */
pos_key_t number_1;
pos_key_t number_2;
pos_key_t number_3;
pos_key_t number_4;
pos_key_t number_5;
pos_key_t number_6;
pos_key_t number_7;
pos_key_t number_8;
pos_key_t number_9;
pos_key_t number_0;
pos_key_t number_del;
pos_key_t number_ture;


/* 主界面上的商家按钮坐标 */
pos_key_t merch;

/* 主界面上的用户按钮坐标 */
pos_key_t user;

/* 返回键 */
pos_key_t back;

/* 提示界面的确定与取消 */
/* 确定 */
pos_key_t ok;
/* 取消 */
pos_key_t cancel;

/* 用户界面上的图标 */
pos_key_t user_msg;     /* 用户信息 */
pos_key_t charge;       /* 充电 */
pos_key_t recharge;     /* 充值 */

/*
    函数名称：set_first_interface
    函数功能：根据图片上的图案，设置触摸屏捕获范围
                当有用户点击图片上的图案，程序对此有响应
    函数参数：None
    返回值：  None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
                图案坐标是根据它在图片的位置，计算出来的
        用户按钮    起始(87, 185)   
                   末尾(248, 366)

        商家按钮   起始(556, 179)   
                  末尾(723, 368)    

*/
void set_first_interface(void)
{

    /* 设置界面坐标 */
    /*
        用户按钮    (87, 185)   
                                (248, 366)

        商家按钮   (556, 179)   
                                (723, 368)    
    */
    merch.x_start = 87;
    merch.x_end = 248;
    merch.y_start = 185;
    merch.y_end = 366;

    user.x_start = 556;
    user.x_end = 723;
    user.y_start = 179;
    user.y_end = 368;

    back.x_start = 0;
    back.x_end = 70;
    back.y_start = 0;
    back.y_end = 56;
}


/*
    函数名称：set_postion_keypad
    函数功能：根据输入键盘的显示位置，设置每个按键的在触摸屏的坐标
             当有用户点击图片上的图案，程序对此有响应
             该输入键盘为3*4结构。
    函数参数：None
    返回值：  None
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
                图案坐标是根据它在图片的位置，计算出来的
                输入键盘是在屏幕的左下角显示
        键盘图像的宽 280
                 高 310
        因此，显示的坐标位 0，480-310
*/
void set_postion_keypad(void)
{
    /* 
        输入键盘是在屏幕的左下角显示
        键盘图像的宽 280
                 高 310
        因此，显示的坐标位 0，480-310
        

    */
    /* 数字1 */
    number_1.x_start = 0;
    number_1.x_end = 280/3;
    number_1.y_start = (480-310);
    number_1.y_end = 310/4 + (480-310);

    /* 数字2 */
    number_2.x_start = 280 / 3;
    number_2.x_end = 280/3 * 2;
    number_2.y_start = (480-310);
    number_2.y_end = 310/4 + (480-310);

    /* 数字3 */
    number_3.x_start = 280/3 * 2;
    number_3.x_end = 280;
    number_3.y_start = (480-310);
    number_3.y_end = 310/4 + (480-310);

    /* 数字4 */
    number_4.x_start = 0;
    number_4.x_end = 280/3;
    number_4.y_start = 310/4 + (480-310);
    number_4.y_end = 310/4 * 2 + (480-310);

    /* 数字5 */
    number_5.x_start = 280/3;
    number_5.x_end = 280/3 * 2;
    number_5.y_start = 310/4 + (480-310);
    number_5.y_end = 310/4 * 2 + (480-310);

    /* 数字6 */
    number_6.x_start = 280/3 * 2;
    number_6.x_end = 280/3 * 3;
    number_6.y_start = 310/4 + (480-310);
    number_6.y_end = 310/4 * 2 + (480-310);

    /* 数字7 */
    number_7.x_start = 0;
    number_7.x_end = 280 / 3;
    number_7.y_start = 310/4 * 2 + (480-310);
    number_7.y_end = 310/4 * 3 + (480-310);

     /* 数字8 */
    number_8.x_start = 280 / 3;
    number_8.x_end = 280 / 3 * 2;
    number_8.y_start = 310/4 * 2 + (480-310);
    number_8.y_end = 310/4 * 3 + (480-310);

    /* 数字9 */
    number_9.x_start = 280 / 3 * 2;
    number_9.x_end = 280;
    number_9.y_start = 310/4 * 2 + (480-310);
    number_9.y_end = 310/4 * 3 + (480-310);

    /* 删除键 */
    number_del.x_start = 0;
    number_del.x_end = 280/3;
    number_del.y_start = 310/4 * 3 + (480-310);
    number_del.y_end = 310/4 * 4 + (480-310);

    /* 数字0 */
    number_0.x_start = 280/3;
    number_0.x_end = 280/3 * 2;
    number_0.y_start = 310/4 * 3 + (480-310);
    number_0.y_end = 310/4 * 4 + (480-310);

    /* 确认键 */
    number_ture.x_start = 280/3 * 2;
    number_ture.x_end = 280/3 * 3;
    number_ture.y_start = 310/4 * 3 + (480-310);
    number_ture.y_end = 310/4 * 4 + (480-310);

}


/*
    函数名称：get_uset_intput
    函数功能：根据用户在输入数字键盘的输入
             通过触摸坐标的范围，返回相应 0 ~ 9 的数字
             当用户按下x图案是，删除用户最后输入的一位数字
    函数参数：None
    返回值： 用户在数字键盘触摸相应的图案返回对应的0 ~ 9的数字
            按下x返回11
            按下✔时返回12
            按下返回键返回13
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
                图案坐标是根据它在图片的位置，计算出来的
                输入键盘是在屏幕的左下角显示
        键盘图像的宽 280
                 高 310
        因此，显示的坐标位 0，480-310
*/
int get_user_intput(int ts_fd)
{
    int x , y;
    TS_GetVal(ts_fd, &x, &y);
    /* 是否数字1 */

    if(x > number_1.x_start && x < number_1.x_end && y > number_1.y_start && y < number_1.y_end)
    {
        printf("get 1\r\n");
        return 1;
    }
    else if(x > number_2.x_start && x < number_2.x_end && y > number_2.y_start && y < number_2.y_end)
    {
        printf("2\r\n");
        return 2;
    }
    else if (x > number_3.x_start && x < number_3.x_end && y > number_3.y_start && y < number_3.y_end)
    {
        printf("3\r\n");
        return 3;
    }
    else if (x > number_4.x_start && x < number_4.x_end && y > number_4.y_start && y < number_4.y_end)
    {
        printf("4\r\n");
        return 4;
    }
    else if (x > number_5.x_start && x < number_5.x_end && y > number_5.y_start && y < number_5.y_end)
    {
        printf("5\r\n");
        return 5;
    }
    else if (x > number_6.x_start && x < number_6.x_end && y > number_6.y_start && y < number_6.y_end)
    {
        printf("6\r\n");
        return 6;
    }
    else if (x > number_7.x_start && x < number_7.x_end && y > number_7.y_start && y < number_7.y_end)
    {
        printf("7\r\n");
        return 7;
    }

    else if (x > number_8.x_start && x < number_8.x_end && y > number_8.y_start && y < number_8.y_end)
    {
        printf("8\r\n");
        return 8;
    }
    else if (x > number_9.x_start && x < number_9.x_end && y > number_9.y_start && y < number_9.y_end)
    {
        printf("9\r\n");
        return 9;
    }
    else if (x > number_0.x_start && x < number_0.x_end && y > number_0.y_start && y < number_0.y_end)
    { 
        printf("0\r\n");
        return 0;
    }
    else if (x > number_del.x_start && x < number_del.x_end && y > number_del.y_start && y < number_del.y_end)
    {
        printf("del\r\n");
        return 11;
    }
    else if (x > number_ture.x_start && x < number_ture.x_end && y > number_ture.y_start && y < number_ture.y_end)
    {
        printf("ture\r\n");
        return 12;
    }
    /* 返回键 */
    else if(x > back.x_start && x < back.x_end && y > back.y_start && y < back.y_end)
    {
        printf("back\r\n");
        return 13;
    }

}


/*
    函数名称：show_number
    函数功能：根据用户在输入数字键盘的输入
             在lcd上显示对应的 0 ~ 9 的数字图片
             当用户按下x图案是，删除用户最后输入的一位数字
    函数参数：None
    返回值： 成功返回0
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
                图案坐标是根据它在图片的位置，计算出来的
                输入键盘是在屏幕的左下角显示
        键盘图像的宽 280
                 高 310
        因此，显示的坐标位 0，480-310
*/
int show_number(LcdInfo_t* lcd_info, int *i, int number, char* file_path, int start_x, int start_y)
{
    char num_file_name[128] = {0};

    printf("i = %d\r\n", *i);

    if((*i) > 11 || (*i) < 0)
    {
        perror("input number error");
        *i = 0;
        return -1;
    }
    switch (number)
    {
    case 0:
        /* 合成数字路径 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i * PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;
    case 1:
        /* 合成数字1的路径 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i * PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;
    case 2:
        /* 合成数字2的路径 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
    break;
    case 3:
        /* 合成数字3 路径 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;

    case 4:
        /* 合成数字4路径 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;
    case 5:
        /* 合成数字5路径 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;
    case 6:
        /* 合成数字6路径 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;
    case 7:
        /* 合成数字7路径 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;
    case 8:
        /* 合成数字8路径 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;
    case 9:
        /* 合成数字路径9 */
        sprintf(num_file_name, "%s/%d.jpg", file_path, number);
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;

    /* 删除键 */
    case 11:
        /* 合成删除键的路径 */
        (*i)--;
        if(*i < 0)
        {
            *i = 0;
            return -1;
        } 
        sprintf(num_file_name, "%s/%s.jpg", file_path, "uname");
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        break;
        
    /* 确认键 */
    case 12:
        /* 按下确认键就进行一步 */
        /* 退出该函数 */
        /* 返回 12 */
        return 12;
    case '.':
        sprintf(num_file_name, "%s/%d.jpg", file_path, "point");
        read_JPEG_file(start_x + *i *PIC_NUM_LEN, start_y, num_file_name, lcd_info);
        (*i)++;
        break;
    
    }
    return 0;
}


/*
    函数名称：set_true_or_false_key
    函数功能：设置提示框上的确定和取消对应图案的坐标
    函数参数：
                @x : x轴的偏移值
                @y : y轴的偏移值
    返回值： 成功返回0
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
    注意事项：
            直接获取提示图片的宽和高，当用户点击图片的2/3的宽，高的 2/3 时定义为确定
            点击剩下的1/3, 高的 2/3 定义为取消
*/
void set_true_or_false_key(int x, int y)
{
    /* 确定键 */
    ok.x_start = x + 500 / 4;
    ok.x_end = x + 500 / 3 * 2;
    ok.y_start = y + 200/3 * 2;
    ok.y_end = y + 200;

    /* 取消键 */
    cancel.x_start = x + 500 / 3 * 2;
    cancel.x_end = x + 500;
    cancel.y_start = y + 200/3 * 2;
    cancel.y_end = y + 200;

}

void set_user_interface_key(void)
{
    user_msg.x_start = 192;
    user_msg.x_end = 537;
    user_msg.y_start = 127;
    user_msg.y_end = 194;

    recharge.x_start = 190;
    recharge.x_end = 537;
    recharge.y_start = 236;
    recharge.y_end = 306;

    charge.x_start = 191;
    charge.x_end = 541;
    charge.y_start = 353;
    charge.y_end = 421;

}