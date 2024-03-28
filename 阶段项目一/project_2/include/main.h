#ifndef MAIN_H
#define MAIN_H

/* Copyright 12/8/2023 Szm */

#include "lcd.h"
#include "ts.h"
#include "jpeg.h"
#include "double_list.h"
#include "key_pad.h"
#include "get_time.h"
#include "file_opt.h"
#include "master.h"
#include "user.h"

/********文件路径**********/
/* 开机动画 */
#define GIF_FIRST_PATH  "/home/shao/nfs/project/image/first_gif"
#define GIF_SEC_PATH  "/home/shao/nfs/project/image/sec_gif"

/* 充电动画 */
/* 快充动画 */
#define GIF_CHARGE_FAST      "/home/shao/nfs/project/image/fast"
/* 普通充电 */
#define GIF_CHARGE_ORD      "/home/shao/nfs/project/image/ordinary"

/* 电枪就绪动图 */
#define GIF_PREPARE      "/home/shao/nfs/project/image/prepare"
/* 支付完成动图 */


/* 待机界面 */
#define PIC_STAN         "/home/shao/nfs/project/image/pictrue/1.jpg"

/* 主界面 */
#define PIC_MAIN        "/home/shao/nfs/project/image/pictrue/2.jpg"

/* 点击商家图案的界面 */
#define PIC_MAIN_ERROR  "/home/shao/nfs/project/image/pictrue/main_error.jpg"

/* 用户登录或注册 */
#define PIC_SEC        "/home/shao/nfs/project/image/pictrue/3.jpg"

/* 用户输入账号界面 */
#define PIC_REGISTER_SHOW_ACC   "/home/shao/nfs/project/image/pictrue/4.jpg"
/* 用户输入密码界面 */
#define PIC_REGISTER_SHOW_PASS   "/home/shao/nfs/project/image/pictrue/5.jpg"
/* 用户再次输入密码界面 */
#define PIC_REGISTER_SHOW_PASS_AG   "/home/shao/nfs/project/image/pictrue/6.jpg"

/* 输出键盘界面 */
#define PIC_KEY_PAD             "/home/shao/nfs/project/image/pictrue/7.jpg"

/* 用户选择充电方式界面 */
#define PIC_CHOOSE_CHAR         "/home/shao/nfs/project/image/pictrue/8.jpg"

/* 用户支付界面 */
#define PIC_PAY                 "/home/shao/nfs/project/image/pictrue/9.jpg"

/* 感谢界面 */
#define PIC_THANK               "/home/shao/nfs/project/image/pictrue/10.jpg"

/* 账号输入错误 */
#define PIC_ACCOUNT_INTPUT_ERR  "/home/shao/nfs/project/image/pictrue/11.jpg"

/* 密码输入错误 */
#define PIC_PASSWORD_INTPUT_ERR  "/home/shao/nfs/project/image/pictrue/12.jpg"

/* 注册成功 */
#define PIC_REGISTER_SUC  "/home/shao/nfs/project/image/pictrue/13.jpg"

/* 充值成功 */
#define PIC_RECHAGER_SUC  "/home/shao/nfs/project/image/pictrue/14.jpg"

/* 支付成功 */
#define PIC_PAY_SUC         "/home/shao/nfs/project/image/pictrue/15.jpg"

/* 充电完成 */
#define PIC_CHARGE_SUC         "/home/shao/nfs/project/image/pictrue/16.jpg"

/* 二维码， B站视频传送门 */
#define PIC_QR_CODE         "/home/shao/nfs/project/image/pictrue/17.jpg"

/* 没有足够的余额支付 */
#define PIC_NO_BALANCE       "/home/shao/nfs/project/image/pictrue/18.jpg"

/* 充值界面 */
#define PIC_RECHAGER  "/home/shao/nfs/project/image/pictrue/19.jpg"

/* 结算界面 */
#define PIC_SETTLEMENT  "/home/shao/nfs/project/image/pictrue/20.jpg"

/* 询问是否登录界面 */
#define PIC_IS_LOGIN_NOW  "/home/shao/nfs/project/image/pictrue/21.jpg"

/* 登录成功界面 */
#define PIC_LOGIN_SUC       "/home/shao/nfs/project/image/pictrue/22.jpg"

/* 没有找到账号信息 */
#define PIC_NO_ACC_MESS     "/home/shao/nfs/project/image/pictrue/23.jpg"

/* 密码输入正确 */
#define PIC_PASSWORD_INPUT_SUC  "/home/shao/nfs/project/image/pictrue/24.jpg"

/* 询问是否进行充电 */
#define PIC_IS_CHARGE_NOW        "/home/shao/nfs/project/image/pictrue/25.jpg"

/* 提示该账号已被注册 */
#define PIC_ACCOUNT_REGISTER        "/home/shao/nfs/project/image/pictrue/26.jpg"

/* 商家方面的图片 */
/* 提示当前图片是第一张 */
#define PIC_IS_FIRST_INFO       "/home/shao/nfs/project/image/pictrue/27.jpg"

/* 提示当前图片是最后一张 */
#define PIC_IS_LAST_INFO       "/home/shao/nfs/project/image/pictrue/28.jpg"

/* 注册输入账号界面 */
#define PIC_REG_ACCOUNT       "/home/shao/nfs/project/image/pictrue/29.jpg"

/* 商家登录界面 */
#define PIC_MASTER_LOGIN       "/home/shao/nfs/project/image/pictrue/30.jpg"

/* 商家账号错误提示框 */
#define PIC_MASTER_LOGIN_ERROR       PIC_ACCOUNT_INTPUT_ERR
/* 商家密码错误提示框 */
#define PIC_MASTER_PASSWORD_ERROR       PIC_PASSWORD_INTPUT_ERR
/* 商家登录成功 */
#define PIC_MASTER_LOGIN_SUC       PIC_LOGIN_SUC
/* 商家输入密码界面 */
#define PIC_MASTER_PASSWORD       "/home/shao/nfs/project/image/pictrue/34.jpg"

/* 商家账号 */
#define MASTER_ACCOUT               "1234"
/* 商家密码 */
#define MASTER_PASSWORD             "1234"
/* 用户界面 */
#define PIC_USER_SHOW       "/home/shao/nfs/project/image/pictrue/35.jpg"

/* 询问是否退出登录 */
#define PIC_IS_LOG_OFF      "/home/shao/nfs/project/image/pictrue/36.jpg"

/* 商家功能界面界面 */
#define PIC_MASTER_FUN_FACE      "/home/shao/nfs/project/image/pictrue/37.jpg"

/* 用户登录输入密码界面 */
#define PIC_USER_INPUT_PASS      "/home/shao/nfs/project/image/pictrue/38.jpg"

/* 用户充电完成 */
#define PIC_CHREAGE_COP      "/home/shao/nfs/project/image/pictrue/39.jpg"

/* 当前账号剩余金额 */
#define PIC_USER_ACC_MONEY      "/home/shao/nfs/project/image/pictrue/40.jpg"

/* 数字 */
#define PIC_NUMBER             "/home/shao/nfs/project/image/number_5"    

/* 透明图片的宽度 */
#define PIC_NUM_LEN     45
            
/* 用户数据 */
#define USER_PATH               "/home/shao/nfs/project/user_data" 

/* 账号位数 */
#define ACCOUNT_BITS        11

/* 密码位数 */
#define PASSWORD_BITS       6

/* 充值金额最大位数 */
#define CHARGE_MAX          4

/* 定义充电一秒需要的金额 */
#define CHARGE_MONEY        1

/* 快速充电的时间 */
#define FAST_CHARGE         1

/* 普通充电的时间 */
#define ORDIN_CHARGE         30

/* 数字输入起始坐标 */
#define NUMBER_POS_STA_X      205
#define NUMBER_POS_STA_Y      63

/* 结算界面的显示坐标 */
#define CHARGE_POS_X          320
#define CHARGE_POS_Y          85

/*********函数声明***********/

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
int main_interface_show(Hnode_t* Pmanage ,LcdInfo_t* lcd_info, int ts_fd);



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
char* login_interface_show(Hnode_t* Pmanige, LcdInfo_t* lcd_info, int ts_fd);


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
int register_show(LcdInfo_t* lcd_info, int ts_fd, Hnode_t* Pmange);



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
                         int postion_x, int postion_y);


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
char* login_show(int ts_fd, LcdInfo_t* lcd_info, Hnode_t *Pmanige);


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
                                char * account_name, Hnode_t* Pmanage);


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
int get_amount_spent(Hnode_t* Pmanage, const char* account_name, float *discount, int flag);


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
                                int ts_fd, int flag);



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
    函数版本：V1.1
    优化内容：添加了对充值时间，在链表的写入
*/
int show_pay_interface(Hnode_t* Pmanage, LcdInfo_t* lcd_info, int ts_fd, 
                                            DataType_t* cur_user, float spend);

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
int show_end(LcdInfo_t* lcd_info, int ts_fd);


/* 用户支付完成后显示的界面 */
int show_user_pay_suc(LcdInfo_t* lcd_info, int ts_fd, int remain_money);

#endif // !MAIN_H
