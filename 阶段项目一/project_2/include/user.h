#ifndef USER_H
#define USER_H

#include "main.h"
#include "font.h"

#define SHOW_USER_MSG_IN_USER           1
#define SHOW_USER_MSG_IN_MASTER         2


int user_interface_show(DataType_t* user_info, LcdInfo_t* lcd_info, int ts_fd);

void show_user_mess(int x, int y, int font_size, DataType_t data, 
                        LcdInfo_t* lcd, int mode, unsigned int bitmap_color, unsigned int font_color);

                        /* 用户充值界面 */
int user_recharge_balance(LcdInfo_t* lcd_info, int ts_fd, DataType_t* cur_user, Hnode_t* Pmanage);


#endif // !
