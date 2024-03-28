#ifndef OTHER_H
#define OTHER_H

#include <stdio.h>
#include <string.h>
#include "gui.h"

void deleteChar(char a, char* dest);

char get_user_keypad_intput(void);
void get_local_time(char *time_str);
int array_match(int *dest, int array_len ,int src);

#endif // !O