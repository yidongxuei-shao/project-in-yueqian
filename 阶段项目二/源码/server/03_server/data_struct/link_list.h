#ifndef LINK_LIST_H
#define LINK_LIST_H

/********************************
 * 
 *          头文件
 * 
 * *****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
/*****************************
 * 
 *       结构体变量
 *  
 * ****************************/
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


//可以构造链表有效结点类型，单向链表的有效结点由2部分组成：数据域 + 指针域
typedef struct link_list_node
{
	time_table_t  data;		//数据域
	struct link_list_node * next; 	//指针域
}link_list_node_t;


//可以构造链表中头结点的类型，头结点就作为管理结构体，头结点是可选的!!!!!
typedef struct link_list_head_node
{	
	int  NodeNum;  			//结点的数量
	struct link_list_node * Head; 	//记录首结点地址
	struct link_list_node * Tail; 	//记录尾结点地址
}link_list_head_node_t;

/*********************       END       ************************/

/***********************************
 * 
 *          函数声明
 * 
 * *********************************/
link_list_head_node_t * LinkedList_Create(void);
link_list_node_t * LinkedList_NewNode(time_table_t Data);
bool LinkedList_IsEmpty(link_list_head_node_t *Pmanager);
bool LinkedList_TailInsert(link_list_head_node_t *Pmanager,time_table_t Data);
bool LinkedList_HeadInsert(link_list_head_node_t *Pmanager,time_table_t Data);
void LinkedList_PrintNode(link_list_head_node_t *Pmanager);
bool LinkedList_DelNode(link_list_head_node_t *Pmanager,time_table_t DestVal);

#endif // 