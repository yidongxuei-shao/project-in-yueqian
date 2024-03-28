#include "bmp.h"


int bmp_show(lcd_info_t* lcd_info, const char* file_name, int start_x, int start_y)
{
    /* 打开bmp文件 */
    int bmp_fp = open(file_name, O_RDWR);
    if (bmp_fp < 0)
    {
        printf("open bmp file error\n");
        return -1;
    }
    int bmp_width = 0, bmp_height = 0, bmp_deep = 0;
    
    /* 读取文件头 */
    lseek(bmp_fp, 18, SEEK_SET);            /* 偏移18个字节读取图片的宽度和高度 */
    read(bmp_fp, &bmp_width, 4);
    read(bmp_fp, &bmp_height, 4);
    
    lseek(bmp_fp, 0x1c, SEEK_SET);
    read(bmp_fp, &bmp_deep, 2);
    bmp_deep /= 8;

    printf("bmp_width = %d, bmp_height = %d, bmp_deep = %d\r\n", 
            bmp_width, bmp_height, bmp_deep);

    int bmp_byte = (4 - (bmp_width * bmp_deep) % 4) % 4;
    // int bmp_size = bmp_width * bmp_height * 3 + bmp_byte;
    // printf("bmp_byte = %d\r\n", bmp_byte);

    /* 读取文件RBG信息 */
    char bmp_buf[bmp_width * bmp_height * bmp_deep];
    
    lseek(bmp_fp, 54, SEEK_SET);            /* 偏移54个字节读取图片的RGB信息 */
    read(bmp_fp, bmp_buf, bmp_width * bmp_height * bmp_deep);
    // printf("bmp_size = %.2f\r\n",bmp_size / 1024.0);
    /* 关闭bmp文件 */
    close(bmp_fp);

    int* start_addr = lcd_info->lcd_mp + lcd_info->witch * start_y + start_x; 

    int k = 0;
    for(int i = bmp_height - 1; i >= 0; i--)
    {
        for(int j = 0; j < bmp_width; j++, k+=3)
        {
            *(start_addr + i * lcd_info->witch + j)
                = (bmp_buf[k + 2] << 16) | (bmp_buf[k + 1] << 8) | bmp_buf[k];
        }
        k += bmp_byte;      /* 字节对齐，切记切记 */
    }


    return 0;

}

int bmp_show_1(lcd_info_t* lcd_info)
{
    //4.打开BMP图片并把文件的位置指示器向后偏移54字节(不需要读取图片的信息，只需要读取颜色分量)
	int bmp_fd = open(BMP_PATH_2,O_RDWR);
	if (-1 == bmp_fd)
	{
		perror("open bmp error");
		return -1;
	}
	
	lseek(bmp_fd,54,SEEK_SET); //文件光标向后偏移54字节 

	//5.把BMP图片中像素点的颜色分量读取出来并存储到缓冲区中
	char bmp_buf[800*480*3] = {0};
	read(bmp_fd,bmp_buf,800*480*3);


	//6.把读取出来的BMP像素点颜色分量映射到申请的内存中，相当于对内存地址进行访问即可 *(addr+i)
	int x,y,j=0;
	for (y = 479;y>=0;y--)
	{
		for (x = 0;x<800;x++,j+=3)
		{
			////把BMP图片的每个像素点和LCD屏幕每个像素点进行转换 BMP存储顺序是从下到上，从左到右
			*(lcd_info->lcd_mp+y*800+x) = bmp_buf[j] | (bmp_buf[j+1]<<8) | (bmp_buf[j+2]<<16); 
		}
	}
}