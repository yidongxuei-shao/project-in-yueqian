#ifndef FILE_OPT_H
#define FILE_OPT_H

/* Copyright 12/8/2023 Szm */
/**
 *  该文件的函数是关于文件的读和写
 *  文件内容：
 *            文件内容只有一行（用户数据文件不是给人看的，是给程序看的）
 *  文件内容格式：
 *             开始：11位是账号信息
 *                   6位是用户密码信息
 *                   4位是当前账号剩余的金额
 *                   24位是用户注册时间    
 *                   其余为用户的开始充电时间和结束充电时间
 * 
 *     11位账号      6位密码        4位余额      25位用户充值时间      25位注册时间        其余均为充电时间
 *  -------------|------------|--------------|------------------|-------------------|---------------
 * */

#include "double_list.h"
#include "get_time.h"
#include "main.h"


/*
    函数名称：write_file
    函数功能：向文件中写入用户的账号信息，如果在指定的路径下没有该账号的文件，就创建该文件
                文件名格式：账号名.txt
    函数参数：
                @file_path：文件的路径
                @data：用户的信息
    返回值：    成功返回0，失败返回负数
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int write_file(const char* file_path, DataType_t data);


/*
    函数名称：write_time_in_file
    函数功能：向对应用户文件中写入用户开始充电时间和结束充电时间
    函数参数：
                @account：用户账号
                @time：存储用户的开始充电时间和结束充电时间的结构体变量
                @Pmanage：管理用户信息的链表头节点
                @mode： 写入的是开始还是结束时间
                        可用TIME_START表示开始充电时间
                            TIME_END表示结束充电时间
                @read_or_write：打开文件的权限
    返回值：    成功返回0，失败返回负数
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int write_time_in_file(const char* account, get_time_t* time, Hnode_t* Pmanage, 
                            int mode, char* read_or_write);


/*
    函数名称：read_dir_data
    函数功能：读取指定目录下的文件，并将文件写入链表中
    函数参数：
                @Pmanage：管理用户信息的链表头节点
    返回值：    成功返回0，失败返回负数
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int read_dir_data(Hnode_t* Pmanage);


/*
    函数名称：read_file_data
    函数功能：读取文件里的信息，并将读取到的信息写入链表中
    函数参数：
                @Pmanage：管理用户信息的链表头节点
                @file_name：  读取文件的名称
    返回值：    成功返回0，失败返回负数
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.1
    优化内容：添加了从文件中读取用户充值时间
*/
int read_file_data(Hnode_t* Pmanage, const char *file_name);



/*
    函数名称：write_file_balance
    函数功能：更新用户的余额信息，就是覆盖文本中指定的位置
    函数参数：
                @account：用户账号
                @data：  用户最新余额
    返回值：    成功返回0，失败返回负数
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.1
    优化内容：添加了用户充值时，将充值时间写入链表中
*/
int write_file_balance(Hnode_t* Pmanage ,char* account, unsigned int data);


#endif // !1