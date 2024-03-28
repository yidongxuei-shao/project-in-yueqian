#include "dir_scan.h"


/*************************************************
 * 函数名称：dir_scan
 * 函数功能：扫描指定目录下的文件，返回指定后缀的文件数量
 * 参数：@path:指定目录
 *      @file_suffix:文件名后缀名
 *      @file_number:文件的数量
 * 
 * ************************************************/
int dir_scan(const char *path, const char *file_suffix, 
                    int* file_numuber, char** file_name[])
{
    /* 打开指定的目录 */
    int count = 0;
    DIR *dir = opendir(path);
    if(dir == NULL || file_suffix == NULL)
    {
        perror("open dir");
        return -1;
    }
    /* 切换目录 */
    if(chdir(path) == -1)
    {
        perror("change dir");
        return -1;
    }
    /* 读取目录下的文件 */
    struct dirent *ptr = NULL;
    while((ptr =  readdir(dir)) !=  NULL)
    {
        /* 判断文件名是否以指定后缀结尾 */
        if(strstr(ptr->d_name, file_suffix)!= NULL)
        {
           count++;
        }
    }

    *file_numuber = count;
    closedir(dir);              /* 关闭目录 */
    return 0;
}

int dir_file(const char *path, const char *file_suffix, 
                    int* file_numuber, char ***file_path)
{
    /* 打开指定的目录 */
    int count = 0;
    DIR *dir = opendir(path);
    if(dir == NULL || file_suffix == NULL)
    {
        perror("open dir");
        return -1;
    }
    /* 切换目录 */
    if(chdir(path) == -1)
    {
        perror("change dir");
        return -1;
    }
    /* 申请堆内存 */
    char** file_name = (char**)malloc(sizeof(char*) * 30);

    /* 读取目录下的文件 */
    struct dirent *ptr = NULL;
    while((ptr =  readdir(dir)) !=  NULL)
    {
        /* 判断文件名是否以指定后缀结尾 */
        if(strstr(ptr->d_name, file_suffix)!= NULL)
        {
           file_name[count] = (char*)malloc(sizeof(char) * 30);
           strcpy(file_name[count], ptr->d_name);
           count++;
        }
    }
    *file_numuber = count;
    *file_path = file_name;
    closedir(dir);              /* 关闭目录 */
    return 0;

}

