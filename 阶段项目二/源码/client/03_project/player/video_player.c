/***********************************************
 * 
 *        测试mplayer在开发板的使用
 * 
 * 
 * *********************************************/

#include "video_player.h"

void mplayer_stop(void)
{
    system("killall -KILL mplayer");
}

void mplayer_play(char *path, char *name)
{
    /* 合成路径 */
    char buf[128] = {0};
    sprintf(buf, "mplayer %s/%s -nosound -geometry 9%:19% &", path, name);

    /* 读取指定的路径下的广告文件 */
    system(buf);
}




