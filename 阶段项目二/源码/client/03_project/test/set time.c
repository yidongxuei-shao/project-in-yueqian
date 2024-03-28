#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    char buf[128] = {0};
    char *year = "2012-10-12";
    sprintf(buf, "date -s %s", year);
    system(buf);
    

    return 0;
}