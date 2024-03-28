#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

/* Copyright 12/8/2023 Szm */

/********************************
 * 
 *      系统头文件
 * 
 * ******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <dirent.h>
#include "link_list.h"
/*
	为了方便访问用户的前驱节点和后继节点，本次项目采用
	双向链表作为程序数据存储的载体。
*/

/**********双向链表************/

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
    // link_list_head_node_t *time_table_list_head;     /* 时间表头节点 */
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



//可以构造链表有效结点类型，单向链表的有效结点由3部分组成：数据域 + prev指针域 + next指针域
typedef struct node
{
	user_info_t    data;				//数据域
	struct node * prev; 	        //prev指针域 指向直接前驱
	struct node * next; 	        //next指针域 指向直接后继
}Node_t;


//可以构造双向链表中头结点的类型，头结点就作为管理结构体，头结点是可选的!!!!!
typedef struct HeadNode
{	
	int  NodeNum;  			//结点的数量
	struct node * Head; 	//首结点地址
	struct node * Tail; 	//尾结点地址
}Hnode_t;

/**********************         END         ***********************************/
/**********************************
 * 
 *      剧场信息链表节点
 * 
 * *******************************/
typedef struct theater_node
{
	theater_info_t    data;			//数据域
	struct theater_node * prev; 	        //prev指针域 指向直接前驱
	struct theater_node * next; 	        //next指针域 指向直接后继
}theater_node_t;

typedef struct theater_head_node
{	
	int  NodeNum;  			//结点的数量
	struct theater_node * Head; 	//首结点地址
	struct theater_node * Tail; 	//尾结点地址
}theater_head_node_t;

/*******************************       END         **********************************/

/********************************************************************************************
*
*	函数名称：DoubleLinkedList_Create
*	函数功能：创建带头结点的空的双向链表
*	函数参数：None
*						
*	返回结果：成功 返回指向双向链表头结点的指针   失败 返回NULL    
*	创建日期：02/08/2023
*	函数作者：Szm
*	函数版本：V1.0
*	优化内容：None
*	注意事项：调用该函数时请注意接收该函数的返回值，返回值就是双向链表头结点的地址
*
* ******************************************************************************************/
Hnode_t * DoubleLinkedList_Create(void);


/********************************************************************************************
*
*	函数名称：DoubleLinkedList_NewNode
*	函数功能：创建双向链表的新结点
*	函数参数：
*				@Data:指的是新结点的数据域要存储的值
*						
*	返回结果：成功 返回指向新结点的指针   失败 返回NULL    
*	创建日期：02/08/2023
*	函数作者：Szm
*	函数版本：V1.1
*	优化内容：V1.1：添加了用户充值时间
*	注意事项：该函数不会单独调用，一般会在插入结点的函数接口中调用，记得接收函数返回值
*				该节点的数据域是自定义类型结构体，具体查看double_list.h
*
* *****************************************************************************************/
Node_t * DoubleLinkedList_NewNode(user_info_t Data);


/********************************************************************************************
*
*	函数名称：DoubleLinkedList_IsEmpty
*	函数功能：判断双向链表中是否存在有效结点
*	函数参数：
*				@Pmanager:指的是指向双向链表的头结点的指针
*						
*	返回结果：成功 返回true   失败 返回false    
*	函数作者：Szm
*	创建日期：02/08/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* ******************************************************************************************/
bool DoubleLinkedList_IsEmpty(Hnode_t *Pmanager);


/********************************************************************************************
*
*	函数名称：DoubleLinkedList_TailInsert
*	函数功能：把创建的新结点插入到双向链表的尾部
*	函数参数：
*				@Pmanager:指的是指向双向链表的头结点的指针
*				@Data    :指的是新结点的数据域要存储的数值
*						
*	返回结果：成功 返回true   失败 返回false    
*	函数作者：Szm
*	创建日期：02/08/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* ******************************************************************************************/
bool DoubleLinkedList_TailInsert(Hnode_t *Pmanager,user_info_t Data);


/********************************************************************************************
*
*	函数名称：DoubleLinkedList_PrintNode
*	函数功能：遍历打印节点内的数据域的数据
*	函数参数：
*				@Pmanager:指的是指向双向链表的头结点的指针
*						
*	返回结果：None    
*	函数作者：Szm
*	创建日期：13/07/2023
*	函数版本：V1.2
*	优化内容：为了能适配项目需求，对函数内部打印数据进行重写
				打印用户账号、密码、创建日期、余额、用户编号
				开始充电时间、结束充电时间
			V1.2: 添加了用户充值时间
*	注意事项：None
*
* ******************************************************************************************/
void DoubleLinkedList_PrintNode(Hnode_t *Pmanager);


/********************************************************************************************
*
*	函数名称：DoubleLinkedList_Find_Node
*	函数功能：通过指定的账号在链表中找到指定的节点，并返回指定节点的地址
*	函数参数：
*				@Pmanager:指的是指向双向链表的头结点的指针
*				@accont： 用户的账号信息
*	返回结果：返回找到的用户节点的数据域   
*	函数作者：Szm
*	创建日期：06/08/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* ******************************************************************************************/
user_info_t* DoubleLinkedList_Find_Node(Hnode_t *Pmanager, 
                    const char* accont);


/********************************************************************************************
*
*	函数名称：get_picture_info
*	函数功能：读取指定文件的内的信息，并存储在QManager中
*	函数参数：
*				@Qmanager:指的是指向双向链表的头结点的指针
*               @file_path：指定的文件路径
*						
*	返回结果：成功返回读取到的文件数量，失败返回负数    
*	创建日期：02/08/2023
*	函数作者：Szm
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* ******************************************************************************************/
int get_picture_info(Hnode_t * QManager, const char* file_path);

/*******************************************
 * 
 *              剧场信息
 *              函数声明
 * 
 * ****************************************/
theater_head_node_t * DoubleLinkedList_Theater_Create(void);
theater_node_t * DoubleLinkedList_Theater_NewNode(theater_info_t Data);
bool DoubleLinkedList_Theater_TailInsert(theater_head_node_t *Pmanager
                                        ,theater_info_t Data);
bool DoubleLinkedList_Theater_IsEmpty(theater_head_node_t *Pmanager);
bool DoubleLinkedList_Theater_Delete(theater_head_node_t *Pmanager, char* play_name);
theater_info_t* DoubleLinkedList_Theater_Find_Node(theater_head_node_t *Pmanager, 
                        const char* play_name);

void DoubleLinkedList_Theater_PrintNode(theater_head_node_t *Pmanager);

time_table_t* theater_time_table_init(theater_info_t * theater_info, int * array_len);

int init_theater_seat_file(theater_node_t * theater_data_node);


#endif // !1