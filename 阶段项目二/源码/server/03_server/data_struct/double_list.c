#include "double_list.h"

/*
	Copyright 12/8/2023 Szm
	为了方便访问用户的前驱节点和后继节点，本次项目采用
	双向链表作为程序数据存储的载体。
*/
void local_time(char *time_str);
/**********双向链表************/

/*********************************************************
 * 
 *                      用户数据链表
 * 
 * ******************************************************/
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
Node_t * DoubleLinkedList_NewNode(user_info_t Data)
{
	//1.需要为链表的有效结点申请堆内存，进行错误处理,堆内存是匿名内存
	Node_t *newnode = (Node_t *)calloc(1,sizeof(Node_t));
	if (NULL == newnode)
	{
		perror("calloc heap memory for new node error");
		return NULL;
	}

	//2.对新的结点的成员进行初始化，分为数据域 + prev指针域 + next指针域
    
	/* 获取当前系统时间 */
	char time[20] = {0};
    local_time(time);
    strcpy(newnode->data.time_account_create, time);

	/* 写入数据 */
    newnode->data.ticket_number = 0;
	/* 对数据域进行赋值 */
	strcpy(newnode->data.account, Data.account);		/* 账号 */
	strcpy(newnode->data.password, Data.password);		/* 密码 */
    strcpy(newnode->data.time_account_create, Data.time_account_create);
    newnode->data.ticket_number = Data.ticket_number;
    for(int i = 0; i < newnode->data.ticket_number; i++)
    {
        strcpy(newnode->data.user_tickets[i].play_name, Data.user_tickets[i].play_name);
        strcpy(newnode->data.user_tickets[i].price, Data.user_tickets[i].price);
        strcpy(newnode->data.user_tickets[i].time_play, Data.user_tickets[i].time_play);
        strcpy(newnode->data.user_tickets[i].time_start, Data.user_tickets[i].time_start);
        newnode->data.user_tickets[i].theater_number = Data.user_tickets[i].theater_number;
        newnode->data.user_tickets[i].seat_number_x = Data.user_tickets[i].seat_number_x;
        newnode->data.user_tickets[i].seat_number_y = Data.user_tickets[i].seat_number_y;
    }
	newnode->data.balance = Data.balance;				/* 余额 */

	strcpy(newnode->data.time_account_create, Data.time_account_create);	/* 用户创建日期 */

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
bool DoubleLinkedList_TailInsert(Hnode_t *Pmanager,user_info_t Data)
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
		printf("balance = %02.2f\r\n", p->data.balance);
		printf("create time = %s\r\n", p->data.time_account_create);
        printf("tick_number = %d\r\n", p->data.ticket_number);
        for(int i = 0; i < p->data.ticket_number; i++)
        {
            printf("play_name = %s\r\n",p->data.user_tickets[i].play_name);
            printf("price = %s\r\n",p->data.user_tickets[i].price);
            printf("play_time = %s\r\n",p->data.user_tickets[i].time_play);
        }
	
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
user_info_t* DoubleLinkedList_Find_Node(Hnode_t *Pmanager, 
                    const char* accont)
{
	/* 遍历链表，查找账号名是否与之匹配 */
	//1.为了防止链表中首结点地址丢失，所以定义指针变量对首结点地址进行备份
	Node_t * p = Pmanager->Head;


	//2.对链表中所有结点进行遍历，循环的条件是判断结点的next指针域
	while(p)
	{
		if(strcmp(accont, p->data.account) == 0)
		{
			return &p->data;
		}

		p = p->next; //把当前结点的直接后继结点作为新的结点，当结点的next指针域是NULL则退出循环
	}
	return NULL;
}

/********************************       END        **************************************/

/*************************************************
 * 
 *              剧场信息链表
 * 
 * **********************************************/
/********************
 * 
 *      创建管理头节点
 * 
 * ******************/
theater_head_node_t * DoubleLinkedList_Theater_Create(void)
{
	//1.为链表的头结点申请一块堆内存，进行错误处理,堆内存是匿名内存
	theater_head_node_t *Pmanager = 
                (theater_head_node_t *)calloc(1,sizeof(theater_head_node_t));
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

/******************************
 * 
 *      创建新节点
 * 
 * ***************************/
theater_node_t * DoubleLinkedList_Theater_NewNode(theater_info_t Data)
{
	//1.需要为链表的有效结点申请堆内存，进行错误处理,堆内存是匿名内存
	theater_node_t *newnode = (theater_node_t *)calloc(1,sizeof(theater_node_t));
	if (NULL == newnode)
	{
		perror("calloc heap memory for new node error");
		return NULL;
	}

	//2.对新的结点的成员进行初始化，分为数据域 + prev指针域 + next指针域
    strcpy(newnode->data.picture_path, Data.picture_path);      /* 剧照图片 */
    strcpy(newnode->data.play_name, Data.play_name);            /* 剧名 */
    strcpy(newnode->data.seat_number_path, Data.seat_number_path);/* 座位表对应的文件路径 */
    strcpy( newnode->data.price, Data.price);       /* 价格 */
    strcpy(newnode->data.time_start, Data.time_start);
    newnode->data.time_play = Data.time_play;    /* 时长 */
    newnode->data.seat_number = Data.seat_number;   /* 剩余的座位数量 */
    newnode->data.theater_number = Data.theater_number; /* 剧场编号 */
    /* 创建时间表头节点 */
    // newnode->data.time_table_list_head = LinkedList_Create();
    strcpy(newnode->data.time_move_start, Data.time_move_start);
    strcpy(newnode->data.time_move_end, Data.time_move_end);
    /* 营业时长 */
    newnode->data.time_business = Data.time_business;
    /* 初始化时间表文件 */
    init_theater_seat_file(newnode);
    
	/* 指针域 */
	newnode->prev = NULL;	
	newnode->next = NULL;
	//3.由于接收堆内存的变量是局部变量，所以需要把新结点的地址返回
	return newnode;
}

/***********************************
 * 
 *          加入链表中
 * 
 * ********************************/
bool DoubleLinkedList_Theater_TailInsert(theater_head_node_t *Pmanager
                                        ,theater_info_t Data)
{
	//1.创建一个新结点，记得接收函数返回值，也就是新结点的地址
	theater_node_t * newnode = DoubleLinkedList_Theater_NewNode(Data);

	//2.判断当前的链表中是否存在其它的结点，如果不存在，则新结点就作为首结点
	if (DoubleLinkedList_Theater_IsEmpty(Pmanager))
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
/******************************************
 * 
 *          判断链表是否为空
 * 
 * ***************************************/
bool DoubleLinkedList_Theater_IsEmpty(theater_head_node_t *Pmanager)
{
	return (Pmanager->Head == NULL); //也可以通过链表结点数量来判断
}

/*****************************************
 * 
 *       从链表中删除指定的结点   
 * 
 * **************************************/
bool DoubleLinkedList_Theater_Delete(theater_head_node_t *Pmanager, char* play_name)
{
	//1.为了防止链表中首结点地址丢失，所以定义指针变量对首结点地址进行备份
	theater_node_t * phead = Pmanager->Head;

	//2.对链表中的所有结点进行遍历，把每个结点和目标值进行比较，两种情况：找到了 or 未找到
	while(phead)
	{
		//根据结点的数据域来进行分析比较
		if (strcmp(phead->data.play_name, play_name) == 0)
		{
			break;
		}

		//如果当前结点的数据域和目标值不相等
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
		Pmanager->Head = phead->next;
		phead->next->prev = NULL;
		phead->next = NULL;
		free(phead);
	}
	else if(phead == Pmanager->Tail)
	{
		phead->prev->next = NULL;
		Pmanager->Tail = phead->prev;
		phead->prev = NULL;
		free(phead);
	}
	else //中间结点
	{
		phead->prev->next = phead->next;
		phead->next->prev = phead->prev;
		phead->next = NULL;
		phead->prev = NULL;
		free(phead);
	}

	//5.由于删除了链表中的一个结点，所以需要更新头结点中记录结点数量的成员，对应的值-1即可
	Pmanager->NodeNum--;

	return true;
}

/**************************************
 * 
 *          查找指定的节点
 * 
 * ************************************/
theater_info_t* DoubleLinkedList_Theater_Find_Node(theater_head_node_t *Pmanager, 
                        const char* play_name)
{
	/* 遍历链表，查找账号名是否与之匹配 */
	//1.为了防止链表中首结点地址丢失，所以定义指针变量对首结点地址进行备份s
	theater_node_t * p = Pmanager->Head;


	//2.对链表中所有结点进行遍历，循环的条件是判断结点的next指针域
	while(p)
	{
		if(strcmp(play_name, p->data.play_name) == 0)
		{
			// printf("222\r\n");
			return &(p->data);
		}

		p = p->next; //把当前结点的直接后继结点作为新的结点，当结点的next指针域是NULL则退出循环
	}
    printf("can not find node\r\n");
	return NULL;
}

/*****************************
 * 
 *         打印节点信息
 * 
 * **************************/
void DoubleLinkedList_Theater_PrintNode(theater_head_node_t *Pmanager)
{

	//1.为了防止链表中首结点地址丢失，所以定义指针变量对首结点地址进行备份
	theater_node_t * p = Pmanager->Head;

	//1.对链表中所有结点进行遍历，循环的条件是判断结点的next指针域
	while(p)
	{
        printf("-------------------------------------------------\r\n");
		printf("play_name = %s\r\n", p->data.play_name);
		printf("theater_number = %d\r\n", p->data.theater_number);
        printf("seat_number = %d\r\n", p->data.seat_number);
        printf("time_play = %d\r\n", p->data.time_play);
        printf("time_start = %s\r\n", p->data.time_start);
        printf("time_business = %d\r\n", p->data.time_business);
        printf("time_play = %d\r\n", p->data.time_play);
        printf("-------------------------------------------------\r\n");
		p = p->next; //把当前结点的直接后继结点作为新的结点，当结点的next指针域是NULL则退出循环
	}
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




void local_time(char *time_str)
{
    time_t t;
    struct tm *tm;
    time(&t);
    tm = localtime(&t);
    sprintf(time_str, "%d-%02d-%02d %02d:%02d:%02d", 
            tm->tm_year + 1900, tm->tm_mon + 1, 
            tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    
}


/********************************************
 * 
 *            根据影片的播放时间和
 *      剧院的营业时长，计算每个影片的开始时间
 *              并将其汇总为时间表
 * *****************************************/
time_table_t* theater_time_table_init(theater_info_t * theater_info, int * array_len)
{
    /* 验证参数的有效性 */
    if(theater_info == NULL)
    {
        perror("theater_time_table_init: input argement error");
        return NULL;
    }
  
    /* 向时间表链表添加数据 */
    int count_all = theater_info->time_business * 60;  /* 分钟，20小时 */
    printf("count_all = %d\r\n", count_all);
    /* 剧片时长 + 15分钟的清洁 */
    int count_time = theater_info->time_play + 15;
    printf("count_time = %d\r\n", count_time);
    /* 计算持续 */
    int total_time = count_all / count_time;    /* 在这时间内总共播放次数 */
    printf("total_time = %d\r\n", total_time);

    /* 申请堆内存 */
    time_table_t *time_table_array = (time_table_t *)calloc(sizeof(time_table_t), total_time);
    if(time_table_array == NULL)
    {
        perror("theater_time_table_init: calloc error");
        return NULL;
    }
    /* 向时间表添加节点 */
    /*********************
     *  时间格式 小时：分钟
     * ******************/
    /* 分割开始时间 */
    int start_hour = 0, start_min = 0;
    /* 以:为分隔符 */
    char *hour = strtok(theater_info->time_start, ":");
    char* min = strtok(NULL, ":");
    /* 转换为整数 */
    start_hour = atoi(hour);
    start_min = atoi(min);
    printf("start hour = %d, start min = %d\r\n", start_hour, start_min);
    /* 生成时间表 */
    int time_table = start_hour * 60 + start_min;   /* 分钟 */
    char time_start[15] = {0};
    char time_end[15] = {0};
    for(int i = 0; i < total_time; i ++)
    {
        if(time_table >= 24 * 60)
        {
            time_table -= 24 * 60;
        }
        /* 将开始时间写入时间表中 */
        memset(time_start, 0, sizeof(time_start));
        sprintf(time_start, "%02d:%02d", time_table / 60, time_table % 60);

        time_table += theater_info->time_play;
        if(time_table >= 24 * 60)
        {
            time_table -= 24 * 60;
        }
        /* 计算影片的开始时间和结算时间 */
        memset(time_end, 0, sizeof(time_end));
        sprintf(time_end, "%02d:%02d", time_table / 60, time_table % 60);

        time_table += 15;

        strcpy(time_table_array[i].time_start, time_start);
        strcpy(time_table_array[i].time_end, time_end);
        /* 添加节点 */
        // LinkedList_TailInsert(theater_info->time_table_list_head, time_table);
    }
    *array_len = total_time;
    return time_table_array;
}

/******************************************
 * 
 *      初始化剧场数据的座位表文件
 * 
 * ***************************************/
int init_theater_seat_file(theater_node_t * theater_data_node)
{
    /* 创建对应剧场的座位表文件目录 */
    printf("path = %s\r\n", theater_data_node->data.seat_number_path);
    if(mkdir(theater_data_node->data.seat_number_path, 0777) < 0)  /* 路径：总路径/剧名/ */
    {
        perror("init_theater_seat_file: mkdir error");
        return -1;
    }
    printf("init_theater_seat_file: create dir %s success\n", theater_data_node->data.seat_number_path);
    /* 初始化时间表 */
    int array_len = 0;
    time_table_t* time_table = theater_time_table_init(&theater_data_node->data, &array_len);
    if(time_table == NULL)
    {
        printf("theater_time_table_init error\n");
        return -1;
    }

    char file_path[128] = {0};
    /* 
        在指定的目录下创建文件
        文件名时影片的开始时间
    */
    for(int i = 0; i < array_len; i++)
    {
        int oped_fd = 0;
        /* 打开文件，没有就创建 */
        memset(file_path, 0, sizeof(file_path));
        sprintf(file_path, "%s/%s", 
                    theater_data_node->data.seat_number_path, time_table[i].time_start);
        printf("file_path = %s\r\n", file_path);
        oped_fd = open(file_path, O_RDWR | O_CREAT, 0666);
        if(oped_fd == -1)
        {
            printf("open file error\n");
            return -1;
        }
        for(int i = 0; i < theater_data_node->data.seat_number; i++)
        {
            /* 写入数据 */
            int write_num = write(oped_fd, "0", 1);
            if(write_num == -1)
            {
                printf("write file error\n");
                return -1;
            }
        }
        close(oped_fd);
    }
    return 0;
}
