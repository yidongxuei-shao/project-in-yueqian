#include "link_list.h"
/*******************************************************************************************
*
*	该例程是为了验证链式存储的线性表的设计方案，为了方便对单向链表中的数据元素进行访问，所以
*	需要记录链表3个重要指标
*
*	链表的英文是LinkedList，设计程序的时候标识符可以采用驼峰命名法进行命名，假设当前的顺
*	序表示为了存储int型的数据，则具体如下
* 	
* *****************************************************************************************/
/********************************************************************************************
*
*	函数名称：LinkedList_Create
*	函数功能：创建带头结点的空的单向链表
*	函数参数：None
*						
*	返回结果：成功 返回指向单向链表头结点的指针   失败 返回NULL    
*	函数作者：lmx
*	创建日期：13/07/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：调用该函数时请注意接收该函数的返回值，返回值就是单向链表头结点的地址
*
* ******************************************************************************************/
link_list_head_node_t * LinkedList_Create(void)
{
	//1.为链表的头结点申请一块堆内存，进行错误处理,堆内存是匿名内存
	link_list_head_node_t *Pmanager = (link_list_head_node_t *)calloc(1,sizeof(link_list_head_node_t));
	if (NULL == Pmanager)
	{
		perror("Calloc Heap Memory For Pmanager Error");
		return NULL;
	}

	//2.对头结点中的各个成员进行初始化，结点数量+首尾结点的地址
	Pmanager->NodeNum = 0;
	Pmanager->Head = NULL;
	Pmanager->Tail = NULL;

	//3.由于接收堆内存的变量是局部变量，所以需要把头结点的地址返回
	return Pmanager;
}

/********************************************************************************************
*
*	函数名称：LinkedList_NewNode
*	函数功能：创建单向链表的新结点
*	函数参数：
*				@Data:指的是新结点的数据域要存储的值
*						
*	返回结果：成功 返回指向新结点的指针   失败 返回NULL    
*	函数作者：lmx
*	创建日期：13/07/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：该函数不会单独调用，一般会在插入结点的函数接口中调用，记得接收函数返回值
*
* *****************************************************************************************/
link_list_node_t * LinkedList_NewNode(time_table_t Data)
{
	//1.需要为链表的有效结点申请堆内存，进行错误处理,堆内存是匿名内存
	link_list_node_t *newnode = (link_list_node_t *)calloc(1,sizeof(link_list_node_t));
	if (NULL == newnode)
	{
		perror("calloc heap memory for new node error");
		return NULL;
	}

	//2.对新的结点的成员进行初始化，分为数据域 + 指针域
    strcpy(newnode->data.time_start,Data.time_start);
    strcpy(newnode->data.time_end,Data.time_end);

	newnode->next = NULL;	
	//3.由于接收堆内存的变量是局部变量，所以需要把新结点的地址返回
	return newnode;
}

/********************************************************************************************
*
*	函数名称：LinkedList_IsEmpty
*	函数功能：判断单向链表中是否存在有效结点
*	函数参数：
*				@Pmanager:指的是指向单向链表的头结点的指针
*						
*	返回结果：成功 返回true   失败 返回false    
*	函数作者：lmx
*	创建日期：13/07/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* ******************************************************************************************/
bool LinkedList_IsEmpty(link_list_head_node_t *Pmanager)
{
	return (Pmanager->Head == NULL);
}


/********************************************************************************************
*
*	函数名称：LinkedList_TailInsert
*	函数功能：把创建的新结点插入到单向链表的尾部
*	函数参数：
*				@Pmanager:指的是指向单向链表的头结点的指针
*				@Data    :指的是新结点的数据域要存储的数值
*						
*	返回结果：成功 返回true   失败 返回false    
*	函数作者：lmx
*	创建日期：13/07/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* ******************************************************************************************/
bool LinkedList_TailInsert(link_list_head_node_t *Pmanager,time_table_t Data)
{
	//1.创建一个新结点，记得接收函数返回值，也就是新结点的地址
	link_list_node_t * newnode = LinkedList_NewNode(Data);
	//2.判断当前的链表中是否存在其它的结点，如果不存在，则新结点就作为首结点
	if (LinkedList_IsEmpty(Pmanager))
	{
		Pmanager->Head = newnode; //把新结点作为首结点
		Pmanager->Tail = newnode; //把新结点作为尾结点
	}
	//3.如果链表中已经存在其它结点，则把新结点插入到链表的尾部作为新的尾结点
	else
	{
		Pmanager->Tail->next = newnode; //把新结点的地址存放在当前链表的尾结点的指针域
		Pmanager->Tail = newnode; 		//更新头结点中尾结点的地址，因为尾结点地址变了
	}
	//4.由于在链表中新增了一个结点，所以需要对头结点的成员进行更新
	Pmanager->NodeNum++;
    
	return true;
}


/********************************************************************************************
*
*	函数名称：LinkedList_HeadInsert
*	函数功能：把创建的新结点插入到单向链表的头部
*	函数参数：
*				@Pmanager:指的是指向单向链表的头结点的指针
*				@Data    :指的是新结点的数据域要存储的数值
*						
*	返回结果：成功 返回true   失败 返回false    
*	函数作者：lmx
*	创建日期：13/07/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* ******************************************************************************************/
bool LinkedList_HeadInsert(link_list_head_node_t *Pmanager,time_table_t Data)
{
	//1.创建一个新结点，记得接收函数返回值，也就是新结点的地址
	link_list_node_t * newnode = LinkedList_NewNode(Data);

	//2.判断当前的链表中是否存在其它的结点，如果不存在，则新结点就作为首结点
	if (LinkedList_IsEmpty(Pmanager))
	{
		Pmanager->Head = newnode; //把新结点作为首结点
		Pmanager->Tail = newnode; //把新结点作为尾结点
	}
	//3.如果链表中已经存在其它结点，则把新结点插入到链表的尾部作为新的尾结点
	else
	{
		newnode->next  = Pmanager->Head; //把链表的首结点地址备份到新结点的指针域中
		Pmanager->Head = newnode;		 //把新结点的地址备份到头结点中记录首结点地址的成员中
	}

	//4.由于在链表中新增了一个结点，所以需要对头结点的成员进行更新
	Pmanager->NodeNum++;

	return true;
}


/********************************************************************************************
*
*	函数名称：LinkedList_PrintNode
*	函数功能：遍历单向链表的有效结点
*	函数参数：
*				@Pmanager:指的是指向单向链表的头结点的指针
*						
*	返回结果：None    
*	函数作者：lmx
*	创建日期：13/07/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* ******************************************************************************************/
void LinkedList_PrintNode(link_list_head_node_t *Pmanager)
{

	//1.为了防止链表中首结点地址丢失，所以定义指针变量对首结点地址进行备份
	link_list_node_t * p = Pmanager->Head;
    if(p == NULL)
    {
        return ;
    }
	//1.对链表中所有结点进行遍历，循环的条件是判断结点的next指针域
	while(p)
	{
		printf("time start %s, time end %s\n"
                ,p->data.time_start,p->data.time_end);

		p = p->next; //把当前结点的直接后继结点作为新的结点，当结点的next指针域是NULL则退出循环
	}
}


/********************************************************************************************
*
*	函数名称：LinkedList_DelNode
*	函数功能：删除单向链表的指定结点
*	函数参数：
*				@Pmanager:指的是指向单向链表的头结点的指针
*				@DestVal :指的是要删除结点的数据域的目标值	
*					
*	返回结果：成功 返回true   失败 返回false     
*	函数作者：lmx
*	创建日期：13/07/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* *****************************************************************************************/
bool LinkedList_DelNode(link_list_head_node_t *Pmanager,time_table_t DestVal)
{
	//1.为了防止链表中首结点地址丢失，所以定义指针变量对首结点地址进行备份
	link_list_node_t * phead = Pmanager->Head;

	link_list_node_t * temp = NULL; //该变量是用于记录要删除结点的直接前驱的地址

	//2.对链表中的所有结点进行遍历，把结点和目标值进行比较，两种情况：找到了 or 未找到
	while(phead)
	{
		//根据结点的数据域来进行分析比较
		if (strcmp(DestVal.time_start, phead->data.time_start) == 0)
		{
			break;
		}

		//如果当前结点的数据域和目标值不相等
		temp  = phead; 		//先把当前结点地址进行备份
		phead = phead->next;//让phead指针指向当前结点的直接后继，直到遇到NULL则退出 
	}

	//3.分析循环退出条件：有两种情况：提前退出 or 遍历完成
	if (NULL == phead)
	{
		printf("Node Is Not Find!\n");
		return false; //提前退出函数
	}
	
	//4.如果找到了目标结点，此时要根据目标结点的位置进行分析：首结点 or 尾结点 or 中间结点	
	if (phead == Pmanager->Head)  
	{
		Pmanager->Head = phead->next; //要删除首结点，所以需要把首结点直接后继的地址更新到头结点中
		phead->next = NULL;			  //把要删除的首结点的next指针指向NULL，该结点完全脱离链表
		free(phead);				  //释放结点堆内存
	}
	else if(phead == Pmanager->Tail)
	{
		temp->next = NULL; //要删除的结点是尾结点，则需要把尾结点的直接前驱结点中的next指针指向NULL
		free(phead);	   //释放结点堆内存
		Pmanager->Tail = temp; //由于尾结点地址改变，所以需要更新头结点中记录尾结点地址
	}
	else //中间结点
	{
		temp->next = phead->next; //要删除中间结点，则需要让该结点的直接前驱的next指针指向该结点的直接后继地址
		phead->next = NULL; //把要删除的中间结点的next指针指向NULL，该结点完全脱离链表
		free(phead);	    //释放结点堆内存
	}

	//5.由于删除了链表中的一个结点，所以需要更新头结点中记录结点数量的成员，对应的值-1即可
	Pmanager->NodeNum--;

	return true;
}
