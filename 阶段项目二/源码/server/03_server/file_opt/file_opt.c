/************************
 * 
 *      文件操作
 * 
 * *********************/
#include "file_opt.h"

/***************************
 * 
 *      读取指定目录下的文件
 *    并将数据写入用户数据链表中
 * 
 * ************************/
int read_user_data_file(char *dir_path, char *file_name, user_info_t *user_data)
{
    
    char buf[120] = {0};
    /* 合成路径 */
    sprintf(buf, "%s/%s", dir_path, file_name);
    /* 打开指定的文件 */
    // FILE* file_fd = fopen(buf, "rb");
    int open_file = open(buf, O_RDONLY);
    if (open_file == -1)
    {
        printf("open file error\n");
        return -1;
    }

    /* 读取文件 */
    int read_num = read(open_file, user_data, sizeof(user_info_t));
    if (read_num == -1)
    {
        printf("read file error\n");
        return -1;
    }
    close(open_file);
    return 0;
}

int read_theater_data_file(char *dir_path, char *file_name, theater_info_t *theater_data)
{
    char buf[120] = {0};
    /* 合成路径 */
    sprintf(buf, "%s/%s", dir_path, file_name);
    /* 打开指定的文件 */
    int open_file = open(buf, O_RDONLY);
    if (open_file == -1)
    {
        printf("open file error\n");
        return -1;
    }

    /* 读取文件 */
    int read_num = read(open_file, theater_data, sizeof(theater_info_t));
    if (read_num == -1)
    {
        printf("read file error\n");
        return -1;
    }
    close(open_file);
    return 0;
}

int write_user_data_file(char *dir_path, char *file_name, user_info_t *user_data)
{
    char buf[120] = {0};
    /* 合成路径 */
    sprintf(buf, "%s/%s", dir_path, file_name);
    printf("write file = %s\n", buf);
    /* 打开指定文件，没有就创建 */
    int open_fd = open(buf, O_RDWR | O_CREAT, 0666);
    if (open_fd == -1)
    {
        perror("open file error");
        return -1;
    }
    /* 写入数据 */
    lseek(open_fd, 0, SEEK_SET);
    int write_num = write(open_fd, user_data, sizeof(user_info_t));
    if (write_num == -1)
    {
        printf("write file error\n");
        return -1;
    }
    close(open_fd);
}

int write_theater_data_file(char *dir_path, char *file_name, theater_info_t *theater_data)
{
    char buf[120] = {0};
    /* 合成路径 */
    sprintf(buf, "%s/%s", dir_path, file_name);
    /* 打开指定文件，没有就创建 */
    int open_fd = open(buf, O_RDWR | O_CREAT, 0666);
    if (open_fd == -1)
    {
        printf("open file error\n");
        return -1;
    }
    /* 写入数据 */
    int write_num = write(open_fd, theater_data, sizeof(theater_info_t));
    if (write_num == -1)
    {
        printf("write file error\n");
        return -1;
    }
    close(open_fd);
    return 0;
}

int write_theater_time_table_file(char *dir_path, char *file_name, 
                link_list_head_node_t *theater_time_table)
{
    time_table_t time_table[theater_time_table->NodeNum];
    link_list_node_t* temp = theater_time_table->Head;
    for(int i = 0; i < theater_time_table->NodeNum; i++)
    {
        strcpy(time_table[i].time_start, temp->data.time_start);
        strcpy(time_table[i].time_end, temp->data.time_end);
        temp = temp->next;
    }
    /* 写入指定的文件中 */
    char buf[120] = {0};
    /* 合成路径 */
    sprintf(buf, "%s/%s", dir_path, file_name);
    /* 打开指定文件，没有就创建 */
    int open_fd = open(buf, O_RDWR | O_CREAT, 0666);
    if (open_fd == -1)
    {
        printf("open file error\n");
        return -1;
    }
    /* 写入数据 */
    int write_num = write(open_fd, file_name, sizeof(time_table));
    if (write_num == -1)
    {
        printf("write file error\n");
        return -1;
    }
    close(open_fd);
    return 0;
}

/****************************************
 * 
 *   读取指定文件下的时间表，并写入链表中
 * 
 * *************************************/
int read_theater_time_table_file(char *dir_path, char *file_name, 
                link_list_head_node_t *theater_time_table)
{
    time_table_t time_table[theater_time_table->NodeNum];
    /* 读取指定文件 */
    char buf[120] = {0};
    /* 合成路径 */
    sprintf(buf, "%s/%s", dir_path, file_name);
    int open_file = open(buf, O_RDWR);
    if(open_file == -1)
    {
        printf("open file error\n");
        return -1;
    }
    int read_num = read(open_file, time_table, sizeof(time_table));
    if(read_num == -1)
    {
        printf("read file error\n");
        return -1;
    }
    close(open_file);
    /* 加入链表中 */
    link_list_node_t* temp = theater_time_table->Head;
    for(int i = 0; i < theater_time_table->NodeNum; i++)
    {
        strcpy(temp->data.time_start, time_table[i].time_start);
        strcpy(temp->data.time_end, time_table[i].time_end);
        temp = temp->next;
    }
    return 0;
}
/***************************************
 * 
 *        读取指定目录下的文件
 *         并将其插入链表中
 * 
 * *************************************/
int loading_user_data(char *dir_path, Hnode_t *user_data_list)
{
    /* 打开指定的目录 */
    int count = 0;
    DIR *dir = opendir(dir_path);
    if(dir == NULL || user_data_list == NULL)
    {
        perror("open dir");
        return -1;
    }
    /* 切换目录 */
    if(chdir(dir_path) == -1)
    {
        perror("change dir");
        return -1;
    }
    /* 读取目录下的文件 */
    struct dirent *ptr = NULL;
    int open_file = 0;
    char buf[120] = {0};
    user_info_t data;
    while((ptr =  readdir(dir)) !=  NULL)
    {
        if(strstr(ptr->d_name,"."))
        {
            continue;
        }
        read_user_data_file(dir_path, ptr->d_name, &data);
        /* 将数据插入链表中 */
        DoubleLinkedList_TailInsert(user_data_list, data);
    }

    closedir(dir);              /* 关闭目录 */
    return 0;
}

int loading_theater_data(char *dir_path, theater_head_node_t *theater_data_list)
{
    /* 打开指定的目录 */
    int count = 0;
    DIR *dir = opendir(dir_path);
    if(dir == NULL || theater_data_list == NULL)
    {
        perror("open dir");
        return -1;
    }
    /* 切换目录 */
    if(chdir(dir_path) == -1)
    {
        perror("change dir");
        return -1;
    }
    /* 读取目录下的文件 */
    struct dirent *ptr = NULL;
    int open_file = 0;
    char buf[120] = {0};
    theater_info_t data;
    while((ptr =  readdir(dir)) !=  NULL)
    {
        if(strstr(ptr->d_name,"."))
        {
            continue;
        }
        read_theater_data_file(dir_path, ptr->d_name, &data);
        /* 将数据插入链表中 */
        // loading_theater_time_table(THEATER_TIME_DIR, ptr->d_name, data.time_table_list_head);
        DoubleLinkedList_Theater_TailInsert(theater_data_list, data);
    }

    closedir(dir);              /* 关闭目录 */
    return 0;
}

/*****************************
 * 
 *      加载时间表
 * 
 * **************************/
int loading_theater_time_table(char *dir_path, char* file_name ,link_list_head_node_t *theater_time_list)
{
    /* 打开指定的目录 */
    int count = 0;
    DIR *dir = opendir(dir_path);
    if(dir == NULL || theater_time_list == NULL)
    {
        perror("open dir");
        return -1;
    }
    /* 切换目录 */
    if(chdir(dir_path) == -1)
    {
        perror("change dir");
        return -1;
    }
    /* 读取目录下的文件 */
    struct dirent *ptr = NULL;
    int open_file = 0;
    char buf[120] = {0};
    time_table_t data[theater_time_list->NodeNum];
    while((ptr =  readdir(dir)) !=  NULL)
    {
        if(strstr(ptr->d_name,"."))
        {
            continue;
        }
        /* 将数据插入链表中 */
        read_theater_time_table_file(dir_path, file_name, theater_time_list);
    }

    closedir(dir);              /* 关闭目录 */
    return 0;
}
