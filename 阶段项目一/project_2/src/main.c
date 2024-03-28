/*
    Copyright 12/8/2023 Szm
    
    系统启动完成后
    1、初始化lcd、ts，并对lcd屏幕进行地址映射
    2、初始化链表
    3、从指定的文件夹内读取用户文件信息，并存入链表中

    4、加载启动动画
    6、加载待机界面
    7、加载主界面图片，其中捕获触摸屏是否被按压，判断指定的位置是否按压
        1、商家位置被按压，不做处理
        2、用户位置被按压，刷新界面进入下一步
    8、根据上个函数的返回值，判断用户的按下位置，当返回值为1时，说明用户，向进行下一步操作
    9、加载用户登录或注册界面，并根据函数返回值，进行判断是否进行下一步
    10、加载选择充电界面
    11、支付界面
    12、感谢界面
*/

#include "main.h"

/*
    函数名称：main
    函数功能：程序的入口
    函数参数：
                @argc：记录输入参数的个数
                @argv：以字符串的形式记录输入参数的内容
    返回值：    成功返回0，失败返回负数
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int main(int argc, char *buf[])
{
    int x = 0, y = 0;
    int flag = -1;
    /* 程序开始，进行初始化lcd屏和触摸屏 */
    LcdInfo_t * lcd_info = LCD_Init(LCD_PATH);
    if(lcd_info == NULL)
    {
        perror("init lcd error!");
        return -1;
    }

    /* 初始化触摸屏 */
    int ts_fd = TS_Init(TS_PATH);
    if(ts_fd < 0)
    {
        perror("init ts error!");
        return -2;
    }
    /* 创建链表头节点 */
    Hnode_t *Pmanage = DoubleLinkedList_Create();

    /* 读取指定路径下的用户信息，并将其存入链表中 */
    read_dir_data(Pmanage);
   
    printf("init user data successful\r\n");
    DoubleLinkedList_PrintNode(Pmanage);
#if 0
    /* 设备文件初始化后，加载开机动画 */
    GIF_Show(GIF_FIRST_PATH, lcd_info, 30, 0x000000, 0,0);
#endif

    /* 显示待机界面，当有人触摸屏幕，进入下一界面，即，主界面 */
    while(1)
    {
        x = 0, y = 0;
        read_JPEG_file(0,0, PIC_STAN, lcd_info);
get_fisrt_again:
        TS_GetVal(ts_fd, &x, &y);
        if(!(x >300 && x < 553 && y > 300 && y < 369))    /* 用来反映用户是否触摸屏幕，有反应就进入主界面*/
        {
            goto get_fisrt_again;
        }
         

main_interface:     /* 回到用户类型选择界面 */

        /* 主界面展示 */
        flag = main_interface_show(Pmanage ,lcd_info, ts_fd);

        char *account_name = NULL;
user_login:  /* 用户返回界面 */
#if 1
        /* 进入用户界面后，会有登录界面 */
        account_name =  login_interface_show(Pmanage ,lcd_info, ts_fd);
        printf("accout = %s\r\n",account_name );

        /* 如果返回值是NULL，则返回上一界面 */
        if(account_name == NULL)        /* 没有该用户*/
        {
            perror("can not find this user");
            /* 询问是否前往注册 */
            goto main_interface;
        }
        /* 通过返回的账号名，在链表中查找，对应的账号信息 */
        DataType_t *data = DoubleLinkedList_Find_Node(Pmanage, account_name);
        printf("data account = %s\r\n", data->account);
#endif

user_face_show:
        /* 登陆成功，会显示用户界面 */
        flag = user_interface_show(data, lcd_info, ts_fd);
        /* 判断用户触摸坐标选择不同的执行程序 */
        /* 用户信息显示 */
        if(flag == 1)
        {
            /* 调用字库接口，显示当前用户的信息 */
            show_user_mess(200,100, 40, *data, lcd_info, SHOW_USER_MSG_IN_USER
                , getColor(0, 255, 255,255), getColor(0,0,0,0));
            /* 获取返回坐标 */
get_ts_user_msg_show:
            TS_GetVal(ts_fd, &x, &y);
            /* 判段用户是否点击返回坐标 */
            if(x > 0 && x < 40 * 2 && y > 0 && y < 40 * 2)
            {
                goto user_face_show;
            }
            else
            {
                /* 重新获取坐标 */
                goto get_ts_user_msg_show;
            }
        }

        /* 充电 */
        else if(flag == 2)
        {
            /* 显示选择充电类型 */
            /* 显示选择充电类型界面 */
            flag =  choose_charge_type_show(ts_fd, lcd_info, PIC_CHOOSE_CHAR,
                                        account_name, Pmanage);

            if(flag == 2)   /* 快充 */
            {
                /* 播放GIF动图，速度快些 */
                GIF_Show(GIF_CHARGE_FAST, lcd_info, FAST_CHARGE, 0x000000, 210,0);
            }
            else if(flag == 1) /* 普通充电 */
            {
                /* 播放GIF动图，速度慢些 */
                GIF_Show(GIF_CHARGE_ORD, lcd_info, ORDIN_CHARGE, 0x000000, 0 ,0);
            }
            else    /* 返回值为 -1 */
            {
                /* 说明用户按下返回键，返回上一界面，即用户界面显示 */
                printf("goto login\r\n");
                goto user_face_show; 
            }
        }

        /* 充值 */
        else if (flag == 3)
        {
            flag = user_recharge_balance(lcd_info, ts_fd, data, Pmanage);

            /* 充值退出，和充值成功都会返会用户界面 */
            if(flag == -1 || flag == 0)
                goto user_face_show;
            
        }
        /* 退出登录 */
        else
        {
            /* 说明用户按下返回键，返回上一界面，即用户登录选项 */
            /* 询问是否退出登录 */
            read_JPEG_file(150, 100, PIC_IS_LOG_OFF, lcd_info);
            /* 获取坐标 */

user_log_off_again:
            TS_GetVal(ts_fd, &x, &y);
            set_true_or_false_key(150, 100);
            if(x > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)
            {
                /* 退出登录，回到用户选择登录或注册界面 */
                goto user_login;
            }
            else if(x > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end)
            {
                /* 回到用户界面 */
                goto user_face_show;
            }
            else
            {
                /* 无效操作，重新获取当前坐标 */
                goto user_log_off_again;
            }
            printf("goto login\r\n");
            goto user_login; 
        }


        /* 显示充电完成 */
        read_JPEG_file(0,0,PIC_CHREAGE_COP, lcd_info);

get_charge_cop_again:       
        TS_GetVal(ts_fd, &x, &y);

        if(!(x > 313 && x < 481 && y > 246 && y < 357))
        {
            /* 触摸其他位置 */
            goto get_charge_cop_again;
        }

        /* 获取充电时长与充电金额 */
        float discount = 0;
        int time_char = get_amount_spent(Pmanage, account_name, &discount, flag);
        printf("charge = %d\r\n",time_char);
#if 0
    printf("time start = %d ,%d, %d \r\n", data->time_start->hour, data->time_start->min, data->time_start->sec);
    printf("time end = %d ,%d, %d \r\n", data->time_end->hour,data->time_end->min,data->time_end->sec);
#endif

        /* GIF显示完后，显示充电时长，充电金额 */
        flag = show_use_time_mon((float)time_char, data->balance, discount ,lcd_info, ts_fd, flag);  /* 界面显示金额和充电分钟 */

        /* 当用户点击确认后，显示支付方式 */
        flag = show_pay_interface(Pmanage, lcd_info, ts_fd, data, time_char);

        /* 显示支付成功，账号剩余金额 */
        show_user_pay_suc(lcd_info, ts_fd, data->balance);

        // DoubleLinkedList_PrintNode(Pmanage);

        /* 5s跳转感谢支付界面 */
        flag = show_end(lcd_info, ts_fd);
    
    }

    return 0;
}


/*
    函数名称：main_interface_show
    函数功能：程序运行时的主界面，根据用户点击的图标，选择不同的程序分支
    函数参数：
                @lcd_info：记录lcd的参数
                @ts_fd：触摸屏的句柄
    返回值：    下一步返回1
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.1
    优化内容：添加了商家界面的显示
*/
int main_interface_show(Hnode_t* Pmanage ,LcdInfo_t* lcd_info, int ts_fd)
{
    int x, y;
        
    /* 设置主界面的按键位置 */
    set_first_interface();
main_error:
    /* 显示主界面 */
    read_JPEG_file(0,0, PIC_MAIN, lcd_info);

get_point_again:    /* 重新获取坐标 */

     /* 获取主界面按键是否按下 */
    TS_GetVal(ts_fd, &x, &y);   /* 获取依次用户在触摸屏的位置 */
    
    /* 商家 */
    if(x > merch.x_start && x < merch.x_end && y > merch.y_start && y < merch.y_end)
    {
        /* 展示该功能尚未设置图片 */
        // read_JPEG_file(0,0, PIC_MAIN_ERROR, lcd_info);

        // /* 延时5s，返回主界面 */
        // sleep(1);

        show_master(Pmanage, lcd_info, ts_fd);

        goto main_error;     /* 重新返回主界面 */

    }
    else if (x > user.x_start && x < user.x_end && y > user.y_start && y < user.y_end)
    {
        /* 进入下一界面 */
        return 1;
    }
    else
    {
        /* 无效坐标 ，重新回到获取坐标*/
        goto get_point_again;
    }
}


/*
    函数名称：login_interface_show
    函数功能：登录界面显示，根据用户点击的图标，选择不同的程序分支
    函数参数：
                @Pmanige：链表的头节点
                @lcd_info：lcd的参数信息
                @ts_fd：触摸屏的句柄
    返回值：    下一步返回用户账号信息，上一步返回NULL
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
char* login_interface_show(Hnode_t* Pmanige, LcdInfo_t* lcd_info, int ts_fd)
{
    /* 用户登录或者注册界面 */
    read_JPEG_file(0,0, PIC_SEC, lcd_info);
    /* 获取主界面按键是否按下 */
    int x, y;
first_get_ts_val:       /* 最末尾else的错误处理 */
    TS_GetVal(ts_fd, &x, &y);   /* 获取依次用户在触摸屏的位置 */

    /* 返回按键 */
    if(x > back.x_start && x < back.x_end && y > back.y_start && y < back.y_end)
    {
        printf("back\r\n");
        /* 返回上一层，即主界面 */
        /* 重新调用主界面显示函数 */
        // main_interface_show(lcd_info, ts_fd);
        return NULL;
    }

    /* 注册按钮 */
    else if (x > merch.x_start && x < merch.x_end && y > merch.y_start && y < merch.y_end)
    {
        printf("user register ....\r\n");
        /* 显示注册界面 */
        /* 该界面有可以输入电话号码作为注册账号（11位）， 密码（6位）*/
        /* 两次输入密码，检测密码输入是否有误，无误后返回登陆界面，进行登录 */
        set_postion_keypad();

        int i = register_show(lcd_info, ts_fd, Pmanige);    /* 显示注册界面 */
        /* 如果返回值不为0，说明注册失败 */
        if( i == 0)
        {
            /* 弹出界面，询问是否现在登录 */
            read_JPEG_file(150,100,PIC_IS_LOGIN_NOW, lcd_info);
            /* 设置坐标 */
            set_true_or_false_key(150,100);
            /* 捕获坐标 */
get_ts_val_again:   /* 错误处理，重新回到获取坐标 */

            TS_GetVal(ts_fd, &x, &y);
            if(x  > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)   /* 确定 */
            {
                printf("chose login now !!!\r\n");
                /* 是 */
                goto login;
             }
            else if (x  > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end) /* 取消 */
            {
                /* 否，返回上一界面 */
                return NULL;
            }
            else
            {
                /* 无响应，回到重新获取坐标 */
                goto get_ts_val_again;

            }
        }
        else if(i == -1)
            return NULL;    /* 返回上一层 */
           
    }
    /* 登录按钮 */
    else if(x > user.x_start && x < user.x_end && y > user.y_start && y < user.y_end)
    {
login:
        printf("login ....\r\n");
        /* 当用户点击进入登录界面，或者注册完成后进入登录界面 */
        /* 显示登陆界面 */
        set_postion_keypad();

        char* accout_name = login_show(ts_fd, lcd_info, Pmanige);
        printf("accout_name = %s\r\n", accout_name);

        if(accout_name == NULL)   /* 进入下一界面 */
        {
           /* 判断为 NULL ，返回上一界面*/
        
            /* 没有账号，显示没有找到该账号 */
            read_JPEG_file(150,100, PIC_NO_ACC_MESS, lcd_info);
            /* 设置坐标 */
            set_true_or_false_key(150,100);

no_login_mess_ts_get:   /* 重新获取坐标 */

            TS_GetVal(ts_fd, &x, &y);   /* 获取依次用户在触摸屏的位置 */

            /* 150 ，100 是提示图片的偏移值，500，200 是图片的宽和高 */
            if(x > 150 && x < 150 + 500 && y > 100 && y < 200 + 100)    /* 确定或者取消 */
            {
            return NULL;
            }
            else
            {
                /* 无效处理，返回重新获取坐标 */
                goto no_login_mess_ts_get;
            }   
        }
        else if(strcmp(accout_name, "back") == 0)
        {
            /* 回到上一界面 */
            return NULL;
        }
        else
        {
            /* 账号密码正确，登录 */
            /* 显示登录成功界面 */
            read_JPEG_file(150,100, PIC_LOGIN_SUC, lcd_info);
            /* 设置坐标 */
            set_true_or_false_key(150,100);
            TS_GetVal(ts_fd, &x, &y);   /* 获取依次用户在触摸屏的位置 */

            /* 150 ，100 是提示图片的偏移值，500，200 是图片的宽和高 */
            if(x > 150 && x < 150 + 500 && y > 100 && y < 200 + 100)    /* 确定或者取消 */
            {
                return accout_name;
            }   
        }
        
    }
    else
    {
        /* 无动作 */
        goto first_get_ts_val;  /* 返回函数开始第一次获取的坐标 */
    }
}

/* 注册 */

/*
    函数名称：register_show
    函数功能：用户注册界面，当用户点击lcd屏显示的账号、密码后面的位置
                回弹出输入键盘，通过输入键盘，可以输入11位的电话号码作为账号
                6位的密码，当注册成功会弹出注册成功的提示框，
                输入不符合规定长度的信息会弹出对应的错误提示框，
                并生成以电话号码作为文件名的用户文件，用于存储用户信息
    函数参数：
                @Pmanige：链表的头节点
                @lcd_info：lcd的参数信息
                @ts_fd：触摸屏的句柄
    返回值：    下一步返回 0，上一步返回-1
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int register_show(LcdInfo_t* lcd_info, int ts_fd, Hnode_t* Pmange)
{

    int x = 0, y = 0;
    int flag = 0;
input_error:    /* 账号输入错误，重新输入 */
    /* 显示账号注册界面 */
    printf("account register....\r\n");
    char buf_acc[12] = {0};

    flag =  show_input(ts_fd, lcd_info,PIC_REG_ACCOUNT, buf_acc, ACCOUNT_BITS, 
                                NUMBER_POS_STA_X, NUMBER_POS_STA_Y);
    if(flag == -1)
        return -1;  /* 返回上一层 */

    /* 计算输入的有效位数 */
    if(strlen(buf_acc) != ACCOUNT_BITS)
    {
        printf("input account error\r\n");
        /* 账号输入错误界面，里面有两个按键，是，和否 */
        set_true_or_false_key(150,100);
        read_JPEG_file(150,100, PIC_ACCOUNT_INTPUT_ERR, lcd_info);
        /* 是，重新输入，否，退出函数，返回上一层界面 */
        /* 设置坐标 */
        set_true_or_false_key(150,100);
        /* 捕获坐标 */
get_ts_val_again:   /* 错误处理，重新回到获取坐标 */

        TS_GetVal(ts_fd, &x, &y);
        if(x  > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)   /* 确定 */
        {
            goto input_error;
        }
        else if (x  > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end) /* 取消 */
        {
            /* 否，返回上一界面 */
            return -1;     
        }
        else
        {
            /* 无响应，回到重新获取坐标 */
            goto get_ts_val_again;
        }
        
    }

    /* 在链表中，能否找到用户输入的账号信息，是返回上一界面，否继续执行 */
    DataType_t * is_find_user = DoubleLinkedList_Find_Node(Pmange, buf_acc);
    if(is_find_user != NULL)
    {
        /* 显示该账号已被注册 */
        read_JPEG_file(150,100, PIC_ACCOUNT_REGISTER, lcd_info);
        /* 获取坐标 */
get_ts_point_again:        
        TS_GetVal(ts_fd, &x, &y);
        if(x > 150 && x < 150 + 500 && y > 100 && y < 100 + 200)
            return -1;  /* 返回上一层 */
        else
            goto get_ts_point_again;
    }
    
input_password_error:
    printf("input password\r\n");
    char buf_password[7] = {0};
    /* 输入密码 */
    /* 显示输入密码界面 */
    flag = show_input(ts_fd,lcd_info, PIC_REGISTER_SHOW_PASS, buf_password,
                         PASSWORD_BITS, NUMBER_POS_STA_X, NUMBER_POS_STA_Y);
    if(flag == -1)
        return -1;

    /* 再次调用密码密码界面，确认上次输入的密码 */
    printf("input password again\r\n");
    char buf_again_password[7] = {0};

    flag = show_input(ts_fd,lcd_info, PIC_REGISTER_SHOW_PASS_AG, buf_again_password, 
                        PASSWORD_BITS, NUMBER_POS_STA_X, NUMBER_POS_STA_Y);
    if(flag == -1)
        return -1;  /* 返回上一层 */

    printf("ps\r\n");
    /* 比较两次输入的密码是否一样 */
    if(strcmp(buf_password, buf_again_password) != 0)
    {
        /* 重新输入密码 */
        printf("intput password error\r\n");
        /* 显示密码输入错误 */
        read_JPEG_file(150,100, PIC_PASSWORD_INTPUT_ERR, lcd_info);

input_again:
        TS_GetVal(ts_fd, &x, &y);
        if(x > 150 && x < 500 + 150 && y > 100 && y < 100 + 200)
            goto input_password_error;
        else 
        {
            goto input_again;  
        }
    }

    /* 显示密码输入正确 */
    read_JPEG_file(150,100, PIC_PASSWORD_INPUT_SUC, lcd_info);


unkown_data:        /* 用户点击对话框以外的位置，无效，重新获取 */
    TS_GetVal(ts_fd, &x, &y);
    if(x > 150 && x < 500 + 150 && y > 100 && y < 100 + 200)
    {
         printf("write link list\r\n");
        /* 写入链表 */
        /* 调用链表尾插 */
        /* 创建链表对应的数据域变量 */
        DataType_t * data = NULL;
        data = (DataType_t *) calloc(sizeof(DataType_t), 1);
        if(data == NULL)
            return -1;
        /* 拷贝账号 */
        strcpy(data->account, buf_acc);
        /* 拷贝密码 */
        strcpy(data->password, buf_password);

        /* 获取当前时间 */
        get_time_t * now_time = get_time();
        strcpy(data->time_account_create, now_time->time_all);

        /* 写入链表 */
        DoubleLinkedList_TailInsert(Pmange, *data);

        /* 打印链表信息 */
        DoubleLinkedList_PrintNode(Pmange);

        printf("ok 1\r\n");
        /* 将上述文件写入对应的用户文件内 */
        /* 文件名就以 账号名.txt */
        char file_path[80] = {0};
        sprintf(file_path, "/home/shao/nfs/project/user_data/%s.txt", data->account);

        printf("file_path = %s\r\n", data->account);
        
        write_file(file_path, *data);
        return 0;

    }
    else
        /* 无效处理， 获取坐标 */
        goto unkown_data;

}



/* 登录 */
/* 返回值反应程序的运行状态
    最后的参数返回登录用户的具体信息
*/
/*
    函数名称：login_show
    函数功能：用户登录界面，当用户点击lcd屏显示的账号、密码后面的位置
                回弹出输入键盘，通过输入键盘，可以输入11位的电话号码作为账号
                6位的密码，通过读取链表的数据，对用户输入的账号信息进行匹配，查到用户信息，弹出
                用户信息在节点的地址，以便对密码信息进行匹配
                用户输入账号、密码错误时，会弹出对应的提示框
    函数参数：
                @Pmanige：链表的头节点
                @lcd_info：lcd的参数信息
                @ts_fd：触摸屏的句柄
    返回值：    下一步返回 用户的账号，上一步返回字符串"back"，没有找到用户信息返回NULL
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
char* login_show(int ts_fd, LcdInfo_t* lcd_info, Hnode_t *Pmanige)
{
    /* 在链表中查找用户信息，以账号作为查找依据 */
    /* 显示登录界面 */
    int x = 0, y = 0;
    int flag = 0;
input_error:    /* 账号输入错误，重新输入 */
    /* 显示账号注册界面 */
    printf("account login....\r\n");

    char buf_acc[12] = {0};

    flag = show_input(ts_fd, lcd_info, PIC_REGISTER_SHOW_ACC, buf_acc,
                         ACCOUNT_BITS, NUMBER_POS_STA_X, NUMBER_POS_STA_Y);
    if(flag == -1)
        return "back";    /* 返回上一层界面 */

    /* 遍历链表获取指定的用户数据 */
    DataType_t* user_data = DoubleLinkedList_Find_Node(Pmanige, buf_acc);
    if(user_data == NULL)
    {
        perror("can not find user");
        return NULL;
    }

    /* 计算输入的有效位数 */
    printf("ret = %d\r\n", (strcmp(user_data->account, buf_acc) != 0));     //1
    if((strlen(buf_acc) != ACCOUNT_BITS) || (strncmp(user_data->account, buf_acc, ACCOUNT_BITS) != 0))
    {
        printf("input account error\r\n");
        /* 账号输入错误界面，里面有两个按键，是，和否 */
        /* 是，重新输入，否，退出函数，返回上一层界面 */
        set_true_or_false_key(150,100);
        read_JPEG_file(150,100, PIC_ACCOUNT_INTPUT_ERR, lcd_info);
        /* 是，重新输入，否，退出函数，返回上一层界面 */
        /* 设置坐标 */
        set_true_or_false_key(150,100);
        /* 捕获坐标 */
get_ts_val_again:   /* 错误处理，重新回到获取坐标 */

        TS_GetVal(ts_fd, &x, &y);
        if(x  > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)   /* 确定 */
        {
            goto input_error;
        }
        else if (x  > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end) /* 取消 */
        {
            /* 否，跳转主界面 */
            return NULL;     
        }
        else
        {
            /* 无响应，回到重新获取坐标 */
            goto get_ts_val_again;
        }
    }

    /* 显示密码输入界面 */
input_password_error:
    printf("input password\r\n");
    char buf_password[7] = {0};
    /* 输入密码 */
    /* 显示输入密码界面 */
    flag = 0;
    flag = show_input(ts_fd,lcd_info, PIC_REGISTER_SHOW_PASS, buf_password, 
                        PASSWORD_BITS, NUMBER_POS_STA_X,NUMBER_POS_STA_Y);
    if(flag == -1)
        return "back";    /* 返回上一层 */

    /* 判断密码的位数 */
    printf("buf passw = %d\r\n", strlen(buf_password));

    if((strlen(buf_password) != PASSWORD_BITS) || (strcmp(user_data->password, buf_password) != 0))
    {
        /* 重新输入密码 */
        printf("intput password error\r\n");
        /* 显示密码输入错误 */
        set_true_or_false_key(150,100);
        read_JPEG_file(150,100, PIC_PASSWORD_INTPUT_ERR, lcd_info);
        /* 是，重新输入，否，退出函数，返回上一层界面 */
        /* 设置坐标 */
        set_true_or_false_key(150,100);
        /* 捕获坐标 */
get_ts_password_again:   /* 错误处理，重新回到获取坐标 */

        TS_GetVal(ts_fd, &x, &y);
        if(x  > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)   /* 确定 */
        {
            goto input_password_error;
        }
        else if (x  > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end) /* 取消 */
        {
            /* 否，跳转主界面 */
            return NULL;     
        }
        else
        {
            /* 无响应，回到重新获取坐标 */
            goto get_ts_password_again;
        }

    }

    /* 读取链表中的数据，根据输入的信息查找 */
    /* 将指定节点数据返回出去 */
    printf("buf_acc = %s\r\n", buf_acc);
    DataType_t *data = DoubleLinkedList_Find_Node(Pmanige, buf_acc);

    /* 没有相关账号 */
    if(data == NULL)
    {
        perror("can not find account\r\n");
        /* 是否跳转注册界面 或者退出程序*/
        set_true_or_false_key(150,100);
        read_JPEG_file(150,100, PIC_NO_ACC_MESS, lcd_info);
        /* 是，重新输入，否，退出函数，返回上一层界面 */
        /* 设置坐标 */
        set_true_or_false_key(150,100);
        /* 捕获坐标 */
get_account_mess_again:   /* 错误处理，重新回到获取坐标 */

        TS_GetVal(ts_fd, &x, &y);
        if(x  > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)   /* 确定 */
        {
            goto input_error;       /* 重新输入账号 */
        }
        else if (x  > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end) /* 取消 */
        {
            /* 否，结束程序，跳转到主界面 */
            return NULL;     
        }
        else
        {
            /* 无响应，回到重新获取坐标 */
            goto get_account_mess_again;
        }
    }

    /* 有账号 */
    /* 比较密码是否输入正确 */
    printf("password = %s\r\n", buf_password);
    printf("pass = %s\r\n", data->password);
    // printf("cmp --- 2 = %d\r\n", strncmp(data->password, buf_password, PASSWORD_BITS));     // 0

    if(strncmp(data->password, buf_password, PASSWORD_BITS) != 0 )
    {
        /* 密码输入错误 */
        printf("password input failed!\r\n");
        /* 显示错误界面， 是否重新输入 */
        set_true_or_false_key(150,100);
        read_JPEG_file(150,100, PIC_PASSWORD_INTPUT_ERR, lcd_info);
        /* 是，重新输入，否，退出函数，返回上一层界面 */
        /* 设置坐标 */
        set_true_or_false_key(150,100);
        /* 捕获坐标 */
get_password_mess_again:   /* 错误处理，重新回到获取坐标 */

        TS_GetVal(ts_fd, &x, &y);
        if(x  > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)   /* 确定 */
        { 
            /* 是 */
            goto input_password_error;       /* 重新输入密码 */
        }
        else if (x  > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end) /* 取消 */
        {
            /* 否，返回上一界面 */
            /* 否 */
            return  NULL;   /* 返回待机界面 */
        }
        else
        {
            /* 无响应，回到重新获取坐标 */
            goto get_password_mess_again;
        }
    }

    return data->account;
}

/* 账号、密码输入界面 */
/*
    函数名称：show_input
    函数功能：按下触摸屏指定的位置，在lcd任意位置显示3*4的数字输入键盘
    函数参数：
                @pcitrue_path：在显示输入键盘前显示的图片
                @lcd_info：lcd的参数信息
                @ts_fd：触摸屏的句柄
                @buf:   通过输入键盘的输入的数字字符
                @count: 输入的位数
                @postion_x:输入键盘的x轴的偏移值
                @postion_y:输入键盘的y轴的偏移值
    返回值：    上一步返回 -1
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int show_input(int ts_fd, LcdInfo_t* lcd_info, char* pcitrue_path, char* buf, int count,
                         int postion_x, int postion_y)
{
    int x = 0, y = 0;
    int number = 0;
    int is_ok = 0;
    read_JPEG_file(0,0,pcitrue_path,lcd_info);

    /* 当触摸输入框，即弹出输入键盘 */
get_again:
    TS_GetVal(ts_fd, &x, &y);
    if(x > 241 && x < 625 && y > 81 && y < 174)
    {
        /* 呼出输出键盘 */
        read_JPEG_file(89,175,PIC_KEY_PAD,lcd_info);
    }
    else if(x > back.x_start && x < back.x_end && y > back.y_start && y < back.y_end)
    {
        return -1;
    }
    else
        goto get_again;

    set_postion_keypad();

    /* 输入数字信息 */
    for(int i = 0; i < count;)
    {   
        // printf("pp\r\n");
        number = get_user_intput(ts_fd);
        printf("number = %d\r\n", number);
        if(number == 13)
            return -1;      /* 返回上一层 */

        buf[i] = '0' + number;     /* 转成字符 */
        /* 检测用户输入的数字，并将数字对应的图片显示到对应的输入框内 */
        /* 输入框的起始坐标， （267， 100）、（709， 153）*/
        is_ok = show_number(lcd_info, &i, number, PIC_NUMBER, postion_x, postion_y);
        if(is_ok == 12)
            /* 退出循坏 */
            break; 
    }
}



/* 显示选择充电类型界面 */
/*
    函数名称：choose_charge_type_show
    函数功能：在lcd显示充电类型选择，在用户点击界面上的快速充电或者普通充电时，
                会获取当前的开始充电时间，并写入用户文件和链表中。
    函数参数：
                @pic_path：显示图片的路径
                @lcd_info：lcd的参数信息
                @ts_fd：触摸屏的句柄
                @buf:   通过输入键盘的输入的数字字符
                @account_name: 用户的账号
                @Pmanage：管理用户信息的头节点
    返回值：    -1 用户要返回上一界面，-2 获取时间失败 ，
                1 用户选择普通充电 ，2 用户选择快速充电  
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int choose_charge_type_show(int ts_fd, LcdInfo_t* lcd_info, char* pic_path,
                                char * account_name, Hnode_t* Pmanage)
{
    /* 显示选择图片 */
    int x, y;
    read_JPEG_file(0,0,pic_path, lcd_info);
    /* 获取触摸屏x,y的坐标 */
other:
    TS_GetVal(ts_fd, &x, &y);
    /* 分析x,y的坐标 */
    /* 返回按键 */
    if(x > back.x_start && x < back.x_end && y > back.y_start && y < back.y_end)
    {
        /* 返回上一界面 */
        return -1;      /* 返回 */
    }

    /* 快充按钮 */
    else if (x > merch.x_start && x < merch.x_end && y > merch.y_start && y < merch.y_end)
    {

        /* 播放电枪插入动图 */
        GIF_Show(GIF_PREPARE, lcd_info, 30, 0xFFFFFF, 100, 50);
        /* 显示确定界面 */
        // printf("1111\r\n");
        /* 调用时间获取 */
        get_time_t* time = get_time();
        if(time == NULL)
        {
            perror("can not get time! retring...\r\n");
            return -2;
        }
        /* 询问是否进行充电，否，返回上一层 */
        read_JPEG_file(150,100,PIC_IS_CHARGE_NOW,lcd_info);
        set_true_or_false_key(150,100);
is_chose_this_charge:

        TS_GetVal(ts_fd, &x, &y);
        if(x  > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)   /* 确定 */
        { 
            // printf("ok\r\n");
            printf("accout strlen = %d\r\n", strlen(account_name));
            write_time_in_file(account_name, time, Pmanage, TIME_START,"A");
            // DoubleLinkedList_PrintNode(Pmanage);
            /* 退出该函数，返回 1 ->表示选择快充 */
            return 1;
        }
        else if (x  > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end) /* 取消 */
        {
            /* 返回上一层 */
            printf("100100\r\n");
            return -1;
        }
        else
        {
            /* 无响应，回到重新获取坐标 */
            goto is_chose_this_charge;
        }
        
    }
    /* 普通按钮 */
    else if(x > user.x_start && x < user.x_end && y > user.y_start && y < user.y_end)
    {
        /* 播放电枪插入动图 */
        GIF_Show(GIF_PREPARE, lcd_info, 30, 0xFFFFFF, 100, 50);
        /* 显示确定界面 */
        /* 询问是否进行充电，否，返回上一层 */
        read_JPEG_file(150,100,PIC_IS_CHARGE_NOW,lcd_info);
        set_true_or_false_key(150,100);
is_chose_this_charge_short:

        TS_GetVal(ts_fd, &x, &y);
        if(x  > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)   /* 确定 */
        { 

           
            /* 调用时间获取 */
            get_time_t* time = get_time();
            if(time == NULL)
            {
                perror("can not get time! retring...\r\n");
                goto other;  /* 重新输入 */
            }
            write_time_in_file(account_name, time, Pmanage, TIME_START, "A");
            /* 退出该函数，返回 2 -> 表示选择普通充电 */
            return 2;
        }
        else if (x  > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end) /* 取消 */
        {
            /* 返回上一层 */
            printf("2330\r\n");
            return -1;
        }
        else
        { 
            /* 无响应，回到重新获取坐标 */
            goto is_chose_this_charge_short;
        }
    }
    else
    {
        goto other;
    }

}

/* 获取消费金额 */
/*
    函数名称：get_amount_spent
    函数功能：根据用户的开始充电时间和结束充电时间，计算用户消费金额
                并将获取到的结束充电时间，并写入用户文件和链表中。
    函数参数：  
                @Pmanage：管理用户信息的头节点
                @account_name: 用户的账号
    返回值：   用户的充电金额
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int get_amount_spent(Hnode_t* Pmanage, const char* account_name, float *discount, int flag)
{
    /* 再次获取时间 */
    get_time_t* time = get_time();

    /* 充电结束，再次写入文件和链表 */
    write_time_in_file(account_name, time, Pmanage, TIME_END, "A");

    /* 计算充电时长 */
    /* 获取指定的节点 */
    DataType_t* spic_data = DoubleLinkedList_Find_Node(Pmanage, account_name);
    if(spic_data == NULL)
    {
        perror("get node failed!\r\n");
        return -1;
    }
    printf("time start: %d:%d:%d\r\n", spic_data->time_start->hour,
                                        spic_data->time_start->min,spic_data->time_start->sec);
    printf("time end: %d:%d:%d\r\n", spic_data->time_end->hour,
                                        spic_data->time_end->min,spic_data->time_end->sec);

    /* 判断用户开始充电时间，是否在优惠时间内 */
    /* 判断开始充电时间 */
    /*
        早上：8  --- 10                 ---> 0.9
                [8,  20]                ---> 0.95
        中午：10 ---- 20                --->  1
                [10, 8]                 ----> 0.95
        晚上：20 ---- 24 ----  8        ----> 0.8
                [20, 10]                ----> 0.85
    */

    /* 用户是否在早上充电 */
    if(spic_data->time_start->hour > 8 && spic_data->time_end->hour < 10)
    {
        printf("in morning\r\n");
        *discount = 0.9;
    }
     /* 用户在早上和中午之间充电 */
    else if(spic_data->time_start->hour < 10 && spic_data->time_end->hour < 20)
    {
        printf("in Morning and noon\r\n");
        *discount = 0.95;
    }
    /* 用户在中午充电 */
    else if(spic_data->time_start->hour > 10 && spic_data->time_end->hour < 20)
    {
        printf("in noon\r\n");
        *discount = 1;
    }
    /* 用户在中午和晚上充电 */
    else if(spic_data->time_start->hour < 20 && 
                (spic_data->time_end->hour < 24 || spic_data->time_end->hour < 8))
    {
        printf("Afternoon and evening\r\n");
        *discount = 0.9;
    }
    /* 用户在晚上充电 */
    else if(spic_data->time_start->hour > 20 && spic_data->time_end->hour < 8)
    {
        printf("in evening\r\n");
        *discount = 0.8;
    }
    /* 用户在晚上和早上充电 */
    else 
    {
        printf("in evening and morning \r\n");
        *discount = 0.85;
    }
    printf("discount = %.2f\r\n", *discount);

    float charge_money = 0;
    int hour = 0, min = 0, sec = 0;
    hour = spic_data->time_end->hour - spic_data->time_start->hour;
    min = spic_data->time_end->min - spic_data->time_start->min;
    sec = spic_data->time_end->sec - spic_data->time_start->sec;

    printf("huor = %d , min = %d, sec = %d\r\n", hour, min, sec);

    if(flag == 2)   /* 快充 */
    {
         charge_money = (hour * 60 + min * 60 + sec) * (*discount) * CHARGE_MONEY * 4;
    }
    else if(flag == 1)
    {
         charge_money = (hour * 60 + min * 60 + sec) * (*discount) * CHARGE_MONEY;
    }
    
    printf("charge_money = %.1f\r\n", charge_money);

    /* 四舍五入 */
    int ret = (int)(charge_money + 0.5);
    /* 限制充电时间，这有BUG，充电时间会算错 */
    // if(charge_sec > 10000)
    //     charge_sec = 1000;
    return ret;
}

/* 显示充电时长和支付金额 */
/*
    函数名称：show_use_time_mon
    函数功能：在lcd上显示用户结算界面，并将用户的充电时长、消费金额、剩余金额
                显示在lcd屏上
    函数参数：  
                @time：充电时间
                @remain_money: 用户账号剩余金额
                @lcd_info：lcd参数
                @ts_fd：触摸屏的句柄
    返回值：   用户的充电金额
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int show_use_time_mon(float time, int remain_money, float discount, LcdInfo_t* lcd_info, 
                                int ts_fd, int flag)
{
    /* 加载图片，用户结算界面 */
    read_JPEG_file(0, 0 , PIC_SETTLEMENT, lcd_info);

    /* 获取用户坐标 */
    int x, y, i = 0;
    /* 显示数字图片在指定位置 */
    /* 将输入的time转换成字符串 */
    char buf[10] = {0};
    if(flag == 2)   /* 快充 */
    {
        sprintf(buf, "%.0f", time / 4);

    }
    else if (flag == 1)
    {
        sprintf(buf, "%.0f", time);
    }
  
    printf("buf = %s\r\n", buf);
    
    /* 计算数组的有限长度 */
    int len = strlen(buf);
    printf("strlen = %d\r\n", len);

    int number = 0;
    /* 显示时长 */
    for(i = 0; i < len;)
    {
        number =  buf[i] - '0';
        printf("number = %d\r\n", number);
        show_number(lcd_info, &i, number, PIC_NUMBER, CHARGE_POS_X,CHARGE_POS_Y);
       
    }
    printf("time = %d\t\n", number);

    sprintf(buf, "%.0f", time);
    len = strlen(buf);
    /* 显示金额 */
    for(i = 0;i < len;)
    {
        int number =   buf[i] - '0';
        show_number(lcd_info, &i, number, PIC_NUMBER,CHARGE_POS_X,CHARGE_POS_Y + 80);
    }
    printf("momory = %d\t\n", number);

    /* 显示剩余金额 */
    sprintf(buf, "%d", remain_money);
    len = strlen(buf);
    for(i = 0;i < len;)
    {
        int number =   buf[i] - '0';
        show_number(lcd_info, &i, number, PIC_NUMBER,CHARGE_POS_X,CHARGE_POS_Y + (80 * 2));
    }

    /* 显示折扣 */
    discount *= 100;
    sprintf(buf, "%d", (int)discount);
    len = strlen(buf);
    for(i = 0;i < len;)
    {
        int number =   buf[i] - '0';
        show_number(lcd_info, &i, number, PIC_NUMBER,CHARGE_POS_X,CHARGE_POS_Y + (80 * 3) - 10);
    }

retry:
    TS_GetVal(ts_fd, &x, &y);
    /* 确认键 */
    if(x > 311 && x < 480 && y > 395 && y < 454)
    {
        return 1;
    }
    else
    {
        /* 重新捕获坐标 */
        goto retry;
    }
    return 0;
}


/*
    函数名称：show_pay_interface
    函数功能：在lcd上显示用户支付界面，并根据用户的选择分为二维码支付
                和余额支付，当用户选择二维码支付，会在lcd上显示
                一张二维码，可以在main.h中修改二维码图片的路径。
                当用户选择余额支付时，根据链表中存储的信息，计算支付后的金额，
                若用户当前金额不足以支付当前充电金额，程序会跳转到充值界面，给用户进行余额充值
                当余额大于充电金额，会将计算扣除充电金额剩下的金额，再写入文件和链表中。
    函数参数：  
                @lcd_info：lcd参数
                @ts_fd：触摸屏的句柄
                @cur_user：当前用户的节点
                @spend：    用户的消费金额
    返回值：   用户的充电金额
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int show_pay_interface(Hnode_t* Pmanage, LcdInfo_t* lcd_info, int ts_fd, 
                                            DataType_t* cur_user, float spend)
{
    /* 加载图片，用户支付界面 */
    read_JPEG_file(0, 0 , PIC_PAY, lcd_info);
    /* 捕获用户坐标 */
    int x, y;
input_invalid:      /* 无效坐标 */

    TS_GetVal(ts_fd, &x, &y);

    if(x > back.x_start && x < back.x_end && y > back.y_start && y < back.y_end)
    {
        printf("back\r\n");
        /* 返回上一层，即主界面 */
        /* 重新调用主界面显示函数 */
        return -1;
    }

    /* 商家按键与用户按键布局一样 */
    /* 二维码支付按钮 */
    else if (x > merch.x_start && x < merch.x_end && y > merch.y_start && y < merch.y_end)
    {
        /* 显示二维码 */
        read_JPEG_file(0, 0, PIC_QR_CODE, lcd_info);

        /* 5秒后进入下一界面 */
        sleep(5);
        return 0;   
    }
    /* 余额支付按钮 */
    else if(x > user.x_start && x < user.x_end && y > user.y_start && y < user.y_end)
    {
pay_again:
        /* 获取当前用户的信息 */
        printf("first balance = %d\r\n", cur_user->balance);
        if(cur_user->balance - (int)spend > 0)
        {
            // printf("current balance = %.1f\r\n", spend);
            /* 跟新该节点信息 */
            cur_user->balance -=  (int)spend;
            // printf("current balance next= %d\r\n", cur_user->balance);

            /* 显示支付成功 */
            read_JPEG_file(150,100, PIC_PAY_SUC, lcd_info);
            /* 跳转下一界面 */
            set_true_or_false_key(150, 100);
            TS_GetVal(ts_fd, &x, &y);
            if((x > ok.x_start && x < ok.x_end && y > ok.y_start && ok.y_end) ||
               (x > cancel.x_start && x < cancel.x_end && y > cancel.y_start && cancel.y_end))
            {
                printf("pay successful\r\n");
            //    printf("current balance next 3 = %d\r\n", cur_user->balance);
                write_file_balance(Pmanage ,cur_user->account, cur_user->balance);

                return 1;
            }
            else
            {
                /* 当用户点击其他地方，重新执行以上的代码段 */
                goto pay_again;
            }
            
        }
        /* 显示当前余额不足 */
        /* 询问用户是否充值 */
inval:  /* 无动作 */       

        read_JPEG_file(150, 100, PIC_NO_BALANCE, lcd_info);
        TS_GetVal(ts_fd, &x, &y);
        set_true_or_false_key(150, 100);

        if(x > ok.x_start && x < ok.x_end && y > ok.y_start && y < ok.y_end)     /* 确定 */
        {
           /* 进入充值界面 */
           /* 不用写 */

        }
        /* 是进入下界面，否，退出登录，返回待机界面 */
        else if(x > cancel.x_start && x < cancel.x_end && y > cancel.y_start && y < cancel.y_end)   /* 取消 */
        {
            return -2;
        }
        else
        {
            /* 无动作 */
            goto inval;
        }

        /* 进入充值界面 */
        printf("entry charge interface!\r\n");
        // TS_GetVal(ts_fd, &x, &y);
        char buf[50];
       
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

show_charge_suucc_error:    /* 当用户点击其他地方时，重新获取坐标判断 */
        TS_GetVal(ts_fd, &x, &y);
        /* 150 是 图片x轴偏移值， 100 是图片y轴的偏移值 */
        if((x > 150 + 190 && x < 150 + 290 && y > 100 + 100 && y < 100 + 145)
                || (x > 150 + 337 && x < 150 + 434 && y > 100 + 98 && y < 100 + 145))     /* 确定 和 取消 */
        // if(x > 150 && x < 150 + 500 && y > 100 && y < 100 + 200)
        {
            printf("return pay interface!\r\n");
            goto pay_again;
        }
        else
        {
            /* 用户点击的位置不是，确定或者取消，返回重新获取坐标 */
            goto show_charge_suucc_error;
        }

    }
    else
    {
        /* 无动作 */
        goto input_invalid;
    }
}


/* 支付完成后显示 */
/*
    函数名称：show_end
    函数功能：显示感谢界面，延时7秒后，返回-2，
                通过返回值跳转到程序的开头，回到待机界面
    函数参数：  
                @lcd_info：lcd参数
                @ts_fd：触摸屏的句柄
    返回值：   -2 回到程序的待机界面
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int show_end(LcdInfo_t* lcd_info, int ts_fd)
{
    /* 显示感谢使用 */
    read_JPEG_file(0,0,PIC_THANK, lcd_info);
    sleep(4);
    /* 如果由用户按下触摸屏，会到待机界面 */
    // int x = 0, y = 0;
    // TS_GetVal(ts_fd, &x, &y);
    // if(x != 0 && y != 0)
    return -2;
}

/* 用户支付完成后显示的界面 */
int show_user_pay_suc(LcdInfo_t* lcd_info, int ts_fd, int remain_money)
{
    int len = 0;
    int i = 0;
    char buf[30] = {0};
    int x, y;
    /* 显示界面 */
    read_JPEG_file(0,0,PIC_USER_ACC_MONEY, lcd_info);
    /* 显示剩余金额 */
    sprintf(buf, "%d", remain_money);
    len = strlen(buf);
    for(i = 0;i < len;)
    {
        int number =   buf[i] - '0';
        show_number(lcd_info, &i, number, PIC_NUMBER,CHARGE_POS_X,CHARGE_POS_Y + (80 * 1));
    }
retry_again:
    TS_GetVal(ts_fd, &x, &y);
    /* 确认键 */
    if(x > 311 && x < 480 && y > 395 && y < 454)
    {
        return 1;
    }
    else
    {
        /* 重新捕获坐标 */
        goto retry_again;
    }
}

