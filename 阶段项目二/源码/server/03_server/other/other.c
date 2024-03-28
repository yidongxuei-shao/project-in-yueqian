#include "other.h"


/*************************
 * 
 *     获取当前系统时间
 * 
 * ***********************/

void get_local_time(char *time_str)
{
    time_t t;
    struct tm *tm;
    time(&t);
    tm = localtime(&t);
    sprintf(time_str, "%d-%02d-%02d %02d:%02d:%02d", 
            tm->tm_year + 1900, tm->tm_mon + 1, 
            tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    
}
/****************************************
 * 
 *   添加剧场数据到链表和写入指定的文件中
 * 
 * **************************************/
int theater_add_data(theater_head_node_t * theater_head, char* play_name, char * price,
            char* time_start, int time_play, int theater_number, int time_business)
{
    if(theater_head == NULL)
    {
        perror("theater_add_data: input argement error");
        return -1;
    }
    /* 添加剧场数据 */
    theater_info_t data;
    strcpy(data.play_name, play_name);
    strcpy( data.price, price);
    strcpy(data.time_start, time_start);
    data.time_play = time_play;
    data.theater_number = theater_number;
    data.time_business = time_business;
    data.seat_number = 32;

    char buf[60] = {0};
    sprintf(buf, "%s/%s.jpg", THEATER_PIC_DIR, play_name);
    strcpy( data.picture_path, buf);                        /* 剧照的图片 */

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s/%s", THEATER_SEAT_DIR, play_name);
    strcpy(data.seat_number_path, buf);        /* 座位表的目录路径 */

    /* 添加数据到链表中 */
    DoubleLinkedList_Theater_TailInsert(theater_head, data);
    /* 写入文件中 */
    write_theater_data_file(THEATER_DATA_DIR, data.play_name, &theater_head->Tail->data);

    return 0;
}