#ifndef VIDO_PLAYER_H
#define VIDO_PLAYER_H

/**********************************
 * 
 *            头文件
 * 
 * ********************************/

#include "lcd.h"
#include <stdlib.h>
#include <string.h>
#include "read_jpeg.h"
#include "image.h"
#include "touch.h"

/***********************************
 * 
 *          函数声明
 * 
 * *********************************/
void mplayer_stop(void);
void mplayer_play(char *path, char* name);


/***********************************
 * 
 *          音频路径
 * 
 * *********************************/
#define AUDIO_PATH "/home/shao/nfs/03_project/image/video"



#endif // !VIDO