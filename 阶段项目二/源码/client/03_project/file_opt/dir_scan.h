#ifndef _DIR_SCAN_H
#define _DIR_SCAN_H

/*********************************************
 * 
 *              头文件
 * 
 * *******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

/*********************************************
 * 
 *               函数声明         
 * 
 * ******************************************/
int dir_scan(const char *path, const char *file_suffix, 
                    int* file_numuber, char** file_name[]);

int dir_file(const char *path, const char *file_suffix, 
                    int* file_numuber, char ***file_path);


#endif // !_DIR_SCAN_H