#ifndef OTHER_H
#define OTHER_H

/***********************************
 * 
 *          头文件
 * 
 * ********************************/
#include "link_list.h"
#include "double_list.h"
#include <time.h>
#include "file_opt.h"
/**********************************
 * 
 *          函数声明
 * 
 * *******************************/

void get_local_time(char *time_str);
int theater_add_data(theater_head_node_t * theater_head, char* play_name, char * price,
            char* time_start, int time_play, int theater_number, int time_business);

#endif // ! 