#include "double_list.h"

/*
	Copyright 12/8/2023 Szm
	为了方便访问用户的前驱节点和后继节点，本次项目采用
	双向链表作为程序数据存储的载体。
*/

/**********双向链表************/

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
Hnode_t * DoubleLinkedList_Create(void)
{
	//1.为链表的头结点申请一块堆内存，进行错误处理,堆内存是匿名内存
	Hnode_t *Pmanager = (Hnode_t *)calloc(1,sizeof(Hnode_t));
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
Node_t * DoubleLinkedList_NewNode(DataType_t Data)
{
	//1.需要为链表的有效结点申请堆内存，进行错误处理,堆内存是匿名内存
	Node_t *newnode = (Node_t *)calloc(1,sizeof(Node_t));
	if (NULL == newnode)
	{
		perror("calloc heap memory for new node error");
		return NULL;
	}

	//2.对新的结点的成员进行初始化，分为数据域 + prev指针域 + next指针域
	memset(newnode->data.account, 0, sizeof(newnode->data.account));
	memset(newnode->data.password, 0, sizeof(newnode->data.account));
	memset(newnode->data.time_account_create, 0, sizeof(newnode->data.time_account_create));
	memset(newnode->data.time_charge_balance, 0, sizeof(newnode->data.time_charge_balance));

	/* 对时间戳申请堆内存 */
	newnode->data.time_start = (get_time_t *)calloc(sizeof(get_time_t), 1);
	if(newnode->data.time_start == NULL)
	{
		perror("can not alloc time start\r\n");
		return NULL;
	}

	newnode->data.time_end = (get_time_t *)calloc(sizeof(get_time_t), 1);
	if(newnode->data.time_end == NULL)
	{
		perror("can not alloc time end\r\n");
		return NULL;
	}

	/* 写入数据 */
	/* 对数据域进行赋值 */
	strcpy(newnode->data.account, Data.account);		/* 账号 */
	strcpy(newnode->data.password, Data.password);		/* 密码 */

	newnode->data.balance = Data.balance;				/* 余额 */
	newnode->data.count = Data.count;					/* 编号 */

	strcpy(newnode->data.time_account_create, Data.time_account_create);	/* 用户创建日期 */
	strcpy(newnode->data.time_charge_balance, Data.time_charge_balance);	/* 用户充值时间 */

	/* 指针域 */
	newnode->prev = NULL;	
	newnode->next = NULL;
	//3.由于接收堆内存的变量是局部变量，所以需要把新结点的地址返回
	return newnode;
}

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
bool DoubleLinkedList_IsEmpty(Hnode_t *Pmanager)
{
	return (Pmanager->Head == NULL); //也可以通过链表结点数量来判断
}


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
bool DoubleLinkedList_TailInsert(Hnode_t *Pmanager,DataType_t Data)
{
	//1.创建一个新结点，记得接收函数返回值，也就是新结点的地址
	Node_t * newnode = DoubleLinkedList_NewNode(Data);

	//2.判断当前的链表中是否存在其它的结点，如果不存在，则新结点就作为首结点
	if (DoubleLinkedList_IsEmpty(Pmanager))
	{
		Pmanager->Head = newnode;
		Pmanager->Tail = newnode;
	}
	//3.如果链表中已经存在其它结点，则把新结点插入到链表的尾部作为新的尾结点
	else
	{
		Pmanager->Tail->next = newnode;
		newnode->prev = Pmanager->Tail;
		Pmanager->Tail= newnode;
	}
	
	//4.由于在链表中新增了一个结点，所以需要对头结点的成员进行更新
	Pmanager->NodeNum++;

	return true;
}

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
void DoubleLinkedList_PrintNode(Hnode_t *Pmanager)
{

	//1.为了防止链表中首结点地址丢失，所以定义指针变量对首结点地址进行备份
	Node_t * p = Pmanager->Head;

	//1.对链表中所有结点进行遍历，循环的条件是判断结点的next指针域
	while(p)
	{
		printf("account = %s\r\n", p->data.account);
		printf("password = %s\r\n", p->data.password);
		printf("count = %d\r\n", p->data.count);
		printf("balance = %d\r\n", p->data.balance);

		printf("create time = %s\r\n", p->data.time_account_create);
		printf("balance time = %s\r\n", p->data.time_charge_balance);
		printf("time start = %s\r\n", p->data.time_start->time_all);
		printf("time end = %s\r\n", p->data.time_end->time_all);
		

		p = p->next; //把当前结点的直接后继结点作为新的结点，当结点的next指针域是NULL则退出循环
	}
}

/* 查找链表中的数据 */
/********************************************************************************************
*
*	函数名称：DoubleLinkedList_Find_Node
*	函数功能：通过指定的账号在链表中找到指定的节点，并返回指定节点的地址
*	函数参数：
*				@Pmanager:指的是指向双向链表的头结点的指针
*				@accont： 用户的账号信息
*	返回结果：None    
*	函数作者：Szm
*	创建日期：06/08/2023
*	函数版本：V1.0
*	优化内容：None
*	注意事项：None
*
* ******************************************************************************************/
DataType_t* DoubleLinkedList_Find_Node(Hnode_t *Pmanager, const char* accont)
{
	/* 遍历链表，查找账号名是否与之匹配 */
	//1.为了防止链表中首结点地址丢失，所以定义指针变量对首结点地址进行备份
	Node_t * p = Pmanager->Head;


	//2.对链表中所有结点进行遍历，循环的条件是判断结点的next指针域
	while(p)
	{
		if(strncmp(accont, p->data.account, PASS_WOED_BITS) == 0)
		{
			// printf("222\r\n");
			return &(p->data);
		}

		p = p->next; //把当前结点的直接后继结点作为新的结点，当结点的next指针域是NULL则退出循环
	}
	return NULL;
}


/* 获取指定目录内的图片信息，并将其填入链表中 */
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
int get_picture_info(Hnode_t * QManager, const char* file_path)
{
    /* 打开指定目录 */
    DIR* dir = opendir(file_path);
    if(dir == NULL)
    {
        perror("can not open dir");
        return -1;
    }
    /* 切换到指定目录下 */
    chdir(file_path);
    struct dirent *dir_file = NULL;
    int count = 0;

    /* 循坏读取指定目录下的图片信息 */
    while((dir_file = readdir(dir)) != NULL)
    {
        /* 从图片文件中筛选出.jpeg格式的图片 */
        /* 这就用到strstr函数 */
        if(strstr(dir_file->d_name, ".jpg") || strstr(dir_file->d_name, ".jpeg"))
        {
            // printf("name = %s\r\n", dir_file->d_name);
            /* 计算文件的数量 */
            count++;
        }
    }
    closedir(dir);
    return count;
}