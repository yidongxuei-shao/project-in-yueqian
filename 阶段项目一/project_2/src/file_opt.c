#include "file_opt.h"

/**
 * Copyright 12/8/2023 Szm
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
 *     11位账号      6位密码        4位余额      25位用户充值时间    25位注册时间           其余均为充电时间
 *  -------------|------------|--------------|------------------|-------------------|---------------
 * */

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
int write_file(const char* file_path, DataType_t data)
{
    FILE * fd = fopen(file_path, "a+");    /* 以只写，没有文件就创建文件，追加的方式打开文件 */  
    if( fd == NULL)
    {
        perror("can not open file\r\n");
        return -1;
    }
   
    /* 写文件 */
    fwrite(data.account, strlen(data.account), 1,fd);    /* 账号 */
    // fwrite("\n", 1, 1,fd);    /* 换行 */

   
    fwrite(data.password, strlen(data.password), 1, fd);   /* 密码 */
    // fwrite("\n", 1, 1,fd);    /* 换行 */
    char buf[30] = {0};
  
    sprintf(buf,"%04d", data.balance);        /*格式转化*/
   
    fwrite(buf, strlen(buf), 1,fd);     /* 余额 */
    // fwrite("\n", 1, 1,fd);    /* 换行 */

    /* 用NULL占24位， 用户充值时间*/
    memset(buf, 0, sizeof(buf));
    fwrite(buf, 24, 1,fd);      /* 充值时间 */


    /* 用户创建日期 */
    /* 获取时间 */
    get_time_t* time = get_time();
    printf("user create time = %s\r\n", time->time_all);
    printf("user create time strlen = %d\r\n", strlen(time->time_all));
    fwrite(time->time_all, strlen(time->time_all), 1, fd); 

    fclose(fd);

    return 0;
}

/* 在对应的文件写入具体时间，并写入结构体变量中 */
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
                            int mode, char* read_or_write)
{
    char file_name[50] = {0};
    char buf[20] = {0};
    strncpy(buf, account, ACCOUNT_BITS);

    printf("strlen acc = %d\r\n",strlen(account));
    sprintf(file_name, "%s/%s.txt", USER_PATH, buf);

    printf("file name = %s\r\n", file_name);

    int fd = open(file_name, O_WRONLY | O_APPEND);
    if( fd < 0)
    {
        perror("can not open file");
        return -1;
    }

    // FILE* fd = fopen(file_name, read_or_write);
    // if(fd == NULL)
    // {
    //     perror("can not open file");
    //     return -1;
    // }
    /* 找到对应的文件 */
    DataType_t* data = DoubleLinkedList_Find_Node(Pmanage, account);
    if(data == NULL)
    {
        printf("can not find %s\r\n", file_name);
        return -2;
    }
    // printf("1\r\n");
    #if 1
    
    /* 写入对应的节点 */
    if(mode == TIME_START)      /* 写入开始时间 */
    {
        data->time_start->hour = time->hour;
        data->time_start->min = time->min;
        data->time_start->sec = time->sec;
        strcpy(data->time_start->time_all, time->time_all);
        // strcat(time->time_all, "\n");
    }
       
    else if(mode == TIME_END)   /* 写入结束时间 */
    {
        data->time_end->hour = time->hour;
        data->time_end->min = time->min;
        data->time_end->sec = time->sec;
        strcpy(data->time_end->time_all, time->time_all);
        // strcat(time->time_all, "\n");
    }
    #endif

    /* 显示时间 */
    show_time(time);

    /* 写入文件 */
    printf("time->time_all = %s\r\n", time->time_all);
    write(fd, time->time_all, strlen(time->time_all));

    close(fd);
    return 0;
}

/* 读取指定目录下的文件，并将文件写入链表中 */
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
int read_dir_data(Hnode_t* Pmanage)
{
    /* 扫描目录下的文件 */
    /* 打开指定目录 */
    DIR* dir = opendir(USER_PATH);
    if(dir == NULL)
    {
        perror("can not open dir");
        return -1;
    }
    /* 切换到指定目录下 */
    chdir(USER_PATH);
    struct dirent *dir_file = NULL;
    int count = 0;
   
    int fd = 0;
    /* 循坏读取指定目录下的图片信息 */
    while((dir_file = readdir(dir)) != NULL)
    {
        /* 从图片文件中筛选出.jpeg格式的图片 */
        /* 这就用到strstr函数，进行比较该文件名是否有带.txt */
        if(strstr(dir_file->d_name, ".txt"))
        {
            // printf("name = %s\r\n", dir_file->d_name);
            read_file_data(Pmanage, dir_file->d_name);
            /* 计算文件的数量 */
            count++;
        }
    }
    closedir(dir);
    return 0;
}


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
int read_file_data(Hnode_t* Pmanage, const char *file_name)
{
    int fd = open(file_name, O_RDWR);
    if(fd < 0)
    {
        perror("can not open file");
        return -1;
    }
    
    DataType_t Data;

    /* 初始化数据 */
    memset(Data.account,0,sizeof(Data.account));
    memset(Data.password, 0, sizeof(Data.password));
    memset(Data.time_account_create, 0, sizeof(Data.time_account_create));
    memset(Data.time_charge_balance, 0, sizeof(Data.time_charge_balance));

    /* 从头顺序读取读取三组数据 */
    read(fd, Data.account, ACCOUNT_BITS*sizeof(char));       /* 账号 */
    // lseek(fd, 1, SEEK_CUR);     /* 跳开'\n' */

    read(fd, Data.password, PASSWORD_BITS*sizeof(char));     /* 密码 */
    // lseek(fd, 1, SEEK_CUR);     /* 跳开'\n' */
    char buf[5] = {0};

    read(fd, buf, sizeof(unsigned int));                  /* 余额 */
    Data.balance = atoi(buf);
    Data.count = Pmanage->NodeNum;

    /* 从文件中读取用户创建时间 */
    read(fd, Data.time_account_create, 24);

    /* 从文件中读取用户的充值时间 */
    read(fd, Data.time_charge_balance, 24);
    
    /* 插入链表 */
    DoubleLinkedList_TailInsert(Pmanage ,Data);

    close(fd);
    return 0;
}

/* 指定修改（更新）用户的余额信息 */
/*
    函数名称：write_file_balance
    函数功能：更新用户的余额信息，就是覆盖文本中指定的位置
    函数参数：
                @account：用户账号
                @data：  用户最新余额
    返回值：    成功返回0，失败返回负数
    函数作者：Szm
    创建日期：8/9/2023
    函数版本：V1.0
*/
int write_file_balance(Hnode_t* Pmanage ,char* account, unsigned int data)
{
    char file_name[60] ={0};
    char temp[12] = {0};

    /* 拷贝账号11位数据到临时变量temp中去 */
    strncpy(temp, account, ACCOUNT_BITS);

    /* 生成文件路径 */
    sprintf(file_name, "%s/%s.txt", USER_PATH, temp);

    printf("file path = %s\r\n", file_name);
    /* 以可读可写的方式打开文件，这样的方式可以对指定字节的数据进行覆盖 */
    int fd = open(file_name, O_RDWR);  /* 在文件的指定位置，进行修改 */
    if(fd < 0)
    {
        printf("can not open file %s\r\n", file_name);
        return -1;
    }
    /* 偏移光标到指定的余额中 */
    lseek(fd, ACCOUNT_BITS + PASS_WOED_BITS , SEEK_SET);

    char buf[4] = {0};
    printf("balance = %d\r\n", data);
    
    /* 将指定的数据的转化成字符串的形式，写入文本中 */
    sprintf(buf, "%04d", data);
    write(fd, buf, strlen(buf));

    /* 获取时间 */
    get_time_t * now_time = get_time();
    printf("write balance time = %s\r\n", now_time->time_all);
    write(fd, now_time->time_all, strlen(now_time->time_all));

    close(fd);      /* 关闭文件 */

    /* 将获取的时间写入链表中 */
    /* 找到该用户节点 */
    DataType_t* user_data = DoubleLinkedList_Find_Node(Pmanage, account);
    strcpy(user_data->time_charge_balance, now_time->time_all);

    return 0;
}