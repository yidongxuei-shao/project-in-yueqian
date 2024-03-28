#include "other.h"

void deleteChar(char a, char* dest) 
{
    int strLength = strlen(dest); // 计算字符串的长度
    int point = 0; // 字符串位置标号，用于保存当前有效字符位置
    for (int i = 0; i < strLength; i++) 
    {
        if (dest[i] == a) 
        {
            continue; // 若当前字符为要删除的字符，则直接跳过继续进行循环
        } 
        else 
        {
            dest[point] = dest[i]; // 当前字符为非删除字符，则往前覆盖
            point++; // 有效字符标号位置递增
        }
    }
    dest[point] = '\0'; // 循环结束，最后一个字符为结束符
}

/***************************************
 * 
 *      捕获用户在登录界面的坐标，并分析
 * 
 * *************************************/
char get_user_keypad_intput(void)
{
    int accunt_count = 0;
    int password_count = 0;
    while(1)
    {
        /* 获取用户在登录界面的坐标 */
        if(touch_flag != 1)
        {
            // printf("touch_flag = 0\r\n");
            usleep(100 * 1000);
            continue;
        }
        touch_flag = 0;
        /* 输出坐标 */
        // printf("touch_x = %d, touch_y = %d\r\n", touch_x, touch_y);
        /* 分析坐标 */
        /* 用户按下数字键盘 */
        if(touch_x > KEYBOARD_1_X && touch_x < KEYBOARD_1_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_1_Y && touch_y < KEYBOARD_1_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字1 */
            printf("user 1\r\n");
            /* 再指定的位置显示数字1 */
            return '1';
        }
        else if (touch_x > KEYBOARD_2_X && touch_x < KEYBOARD_2_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_2_Y && touch_y < KEYBOARD_2_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字2 */
            printf("user 2\r\n");
            return '2';
        }
        else if (touch_x > KEYBOARD_3_X && touch_x < KEYBOARD_3_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_3_Y && touch_y < KEYBOARD_3_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字3 */
            printf("user 3\r\n");
            return '3';
        }
        else if (touch_x > KEYBOARD_4_X && touch_x < KEYBOARD_4_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_4_Y && touch_y < KEYBOARD_4_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字4 */
            printf("user 4\r\n");
            return '4';
        }
        else if (touch_x > KEYBOARD_5_X && touch_x < KEYBOARD_5_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_5_Y && touch_y < KEYBOARD_5_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字5 */
            printf("user 5\r\n");
            return '5';
        }
        else if (touch_x > KEYBOARD_6_X && touch_x < KEYBOARD_6_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_6_Y && touch_y < KEYBOARD_6_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字6 */
            printf("user 6\r\n");
            return '6';
        }
        else if (touch_x > KEYBOARD_7_X && touch_x < KEYBOARD_7_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_7_Y && touch_y < KEYBOARD_7_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字7 */
            printf("user 7\r\n");
            return '7';
        }
        else if (touch_x > KEYBOARD_8_X && touch_x < KEYBOARD_8_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_8_Y && touch_y < KEYBOARD_8_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字8 */
            printf("user 8\r\n");
            return '8';
        }
        else if (touch_x > KEYBOARD_9_X && touch_x < KEYBOARD_9_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_9_Y && touch_y < KEYBOARD_9_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字9 */
            printf("user 9\r\n");
            return '9';
        }
        else if (touch_x > KEYBOARD_0_X && touch_x < KEYBOARD_0_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_0_Y && touch_y < KEYBOARD_0_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字0 */
            printf("user 0\r\n");
            return '0';
        }
        else if (touch_x > KEYBOARD_DEL_X && touch_x < KEYBOARD_DEL_X + KEYBOARD_WIDTH
            && touch_y > KEYBOARD_DEL_Y && touch_y < KEYBOARD_DEL_Y + KEYBOARD_HEIGHT)
        {
            /* 用户按下数字DEL */
            printf("user DEL\r\n");
            return 'd';
        }
        else if(touch_x > BUTTON_BACK_X && touch_x < BUTTON_BACK_X + BUTTON_BACK_WIDTH
        && touch_y > BUTTON_BACK_Y && touch_y < BUTTON_BACK_Y + BUTTON_BACK_HEIGHT)
        {
            /* 用户按下返回 */
            printf("user back\r\n");
            return 'b';
        }
        else
        {
            printf("key pad other\r\n");
            touch_x = 0;
            touch_y = 0;
            continue;
        }
    }
}

/*************************************
 * 
 *          获取系统时间
 * 
 * ***********************************/
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

/**************************
 * 
 *      数组元素匹配
 * 
 * ***********************/
int array_match(int *dest, int array_len ,int src)
{
    for(int i = 0; i < array_len; i++)
    {
        if(dest[i] == src)
        {
            return 1;
        }
    }
    return -1;
}

