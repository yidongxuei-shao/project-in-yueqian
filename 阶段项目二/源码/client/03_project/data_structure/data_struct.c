#include "data_struct.h"
#include <stdlib.h>
#include <string.h>
/**************************************
 * 
 *      备份删除指定的下标的数组
 *  
 * ************************************/
struct_ticket_t user_ticket_delete(user_info_t* user_info, int index)
{
    struct_ticket_t backup_ticket;
    memset(&backup_ticket, 0, sizeof(struct_ticket_t));
    backup_ticket = user_info->user_tickets[index];

    /* 对指定的下标进行删除 */
    for(int i = index; i < user_info->ticket_number; i++)
    {
        /* 将后面的数据向前移动 */
        user_info->user_tickets[i] = user_info->user_tickets[i + 1];
    }
    /* 最大值减一 */
    user_info->ticket_number--;
    return backup_ticket;
}

/****************************************
 * 
 *      增加用户的票数
 * 
 * *************************************/
int user_ticket_add(user_info_t* user_info, struct_ticket_t tick_info)
{
    /* 在数组的末尾添加数据 */
    user_info->user_tickets[user_info->ticket_number] = tick_info;
    user_info->ticket_number++;
    return 0;
}