#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "read_jpeg.h"
#include "image.h"

int main(void)
{
    lcd_info_t *lcd = lcd_init();
    char buf[128] = {0};
    sprintf(buf, "%s/%s", IMAGE_AD_PATH, "sanzhuonian.jpg");
    read_JPEG_file(buf, lcd->lcd_mp, 20, 50);

    printf("buf = %s\r\n", buf);
    return 0;
}

