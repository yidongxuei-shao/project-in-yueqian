/****************************
 *      
 *  定义了用户数据结构和剧场的数据结构
 * 
 * *************************/

#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

/***************************
 * 
 *        时间表
 *  根据营业时间和影片时长，生成时间表
 *  每场影片之间间隔为15分钟
 * ************************/
typedef struct time_table
{
    char time_start[30];            /* 开始时间 */
    char time_end[30];              /* 结束时间 */
}time_table_t;



/****************************
 * 
 *      影院结构体类型
 * 
 * **************************/
typedef struct theater_info
{
    char play_name[30];             /* 剧名 */
    int time_business;              /* 营业时长 */
    char time_start[30];            /* 剧院开始时间 */
    char price[5];                  /* 价格 */
    int time_play;                  /* 时长 */
    int theater_number;             /* 剧场编号 */
    int seat_number;                /* 剩余的作为数量 */
    char seat_number_path[60];      /* 存储座位布局的文件路径 */
    char picture_path[60];          /* 剧照的图片 */
    char time_move_start[30];            /* 开始时间 */
    char time_move_end[30];              /* 结束时间 */

}theater_info_t;


/*****************************
 * 
 *       门票结构体类型
 * 
 * **************************/
typedef struct struct_ticket
{
    char play_name[30];     /* 剧名 */
    char time_play[30];     /* 时长 */
    char time_start[30];    /* 开始时间 */
    char price[5];          /* 价格 */
    int theater_number;     /* 剧场编号 */
    int seat_number_x;      /* 座位号   x坐标 */
    int seat_number_y;      /* 座位号   y坐标 */

}struct_ticket_t;
/*****************************
 * 
 *          用户结构体
 * 
 * **************************/
typedef struct user_info
{
	char account[20];			/* 记录用户的电话号码，即账号 11位 + '\0' */
	char password[20];			/* 记录用户的密码 */

	double balance;				/* 账号余额 */
	char time_account_create[30];					/* 用户创建时间 */
    
    /* 账号所拥有的门票 */
    int ticket_number;                      /* 用户持有的门票数量 */
    struct_ticket_t user_tickets[10];       /* 用户所拥有的门票 */
	
}user_info_t;

/*******************************************
 *              结构体变量
 *           用于存储天气数据           
 * *****************************************/
typedef struct weather_data{
    int temp;
    char weather[40];
}weather_data_t;


/*******************************
 * 
 *          函数声明
 * 
 * *****************************/
struct_ticket_t user_ticket_delete(user_info_t* user_info, int index);
int user_ticket_add(user_info_t* user_info, struct_ticket_t tick_info);



#endif // !DATA