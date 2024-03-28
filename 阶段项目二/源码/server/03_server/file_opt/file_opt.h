#ifndef FILE_OPT_H
#define FILE_OPT_H

#include "link_list.h"
#include "double_list.h"
/********************************
 * 
 *      用户数据目录
 * 
 * *****************************/
#define USER_DATA_DIR "/home/shao/linux/nfs/03_server/data/user_info"

/********************************
 * 
 *        剧场数据目录
 * 
 * ******************************/
#define THEATER_DATA_DIR    "/home/shao/linux/nfs/03_server/data/theater_info"

/********************************
 * 
 *          剧场座位分布目录
 * 
 * ******************************/
#define THEATER_SEAT_DIR    "/home/shao/linux/nfs/03_server/data/theater_seat"

/********************************
 * 
 *         剧情图片目录
 * 
 * ******************************/
#define THEATER_PIC_DIR    "/home/shao/linux/nfs/03_server/data/theater_picture"

/********************************
 * 
 *          时间表目录
 * 
 * ******************************/
#define THEATER_TIME_DIR    "/home/shao/linux/nfs/03_server/data/theater_time_table"
/********************************************
 * 
 *                函数声明
 * 
 * ******************************************/
int read_user_data_file(char *dir_path, char *file_name, user_info_t *user_data);
int read_theater_data_file(char *dir_path, char *file_name, theater_info_t *theater_data);
int write_user_data_file(char *dir_path, char *file_name, user_info_t *user_data);
int write_theater_data_file(char *dir_path, char *file_name, theater_info_t *theater_data);


int loading_user_data(char *dir_path, Hnode_t *user_data_list);
int loading_theater_data(char *dir_path, theater_head_node_t *theater_data_list);

int write_theater_time_table_file(char *dir_path, char *file_name, 
                link_list_head_node_t *theater_time_table);
int read_theater_time_table_file(char *dir_path, char *file_name, 
                link_list_head_node_t *theater_time_table);
int loading_theater_time_table(char *dir_path, char* file_name ,
                link_list_head_node_t *theater_time_list);

#endif // !FILE_OPT_