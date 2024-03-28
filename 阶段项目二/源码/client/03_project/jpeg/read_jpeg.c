#include "read_jpeg.h"

#if 1
int read_JPEG_file (const char * filename, int* lcd_map, int start_x, int start_y)
{
 
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
 
  /* More stuff */
  FILE * infile;		/* source file */
  unsigned char* buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
    if(lcd_map == NULL)
    {
        perror("lcd mmap error");
        return -1;
    }
  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }
 
  /* Step 1: allocate and initialize JPEG decompression object */
  /* Now we can initialize the JPEG decompression object. */
  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: read file parameters with jpeg_read_header() */

    jpeg_read_header(&cinfo, TRUE);
   
  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

   jpeg_start_decompress(&cinfo);
  
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (unsigned char *)calloc(1, row_stride);
  if(buffer == NULL)
    {
        perror("can not calloc");
        return -1;
    }

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */
 
  while (cinfo.output_scanline < cinfo.output_height) {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
   
    jpeg_read_scanlines(&cinfo, &buffer, 1);
    for(int i = 0; i < cinfo.output_width; i++)
    {
        *(lcd_map + start_x + start_y * 800 + i 
            + (cinfo.output_scanline - 1) * 800)   /* 对像素点进行换行 */
            =  (buffer[i * 3] << 16) | (buffer[i * 3 + 1] << 8) 
                    | buffer[i * 3 + 2]; // LCD -- ARGB   JPG --RGB 
    }

  }

  /* Step 7: Finish decompression */

  (void) jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  fclose(infile);

  return 1;
}
#endif


int show_GIF(lcd_info_t *lcd_info ,const char *dir_name, 
                    int start_x, int start_y, int speed)
{
    if (dir_name == NULL || start_x < 0 || start_y < 0 || speed < 0 
        || start_x >= 800 || start_y >= 480 || lcd_info == NULL)
    {
        perror("intput argument error");
        return -1;
    }
    int file_number = 0;
    /* 计算目录中的以.jpg结尾的文件个数 */
    if (dir_scan(dir_name, ".jpg", &file_number, NULL) < 0)
    {
        perror("dir_scan error");
        return -2;
    }
    printf("file numner = %d\r\n", file_number);
    /* 在一个循坏内将图片读取出来 */
    int file_fd = 0;
    char filename[200] = {0};
    for (int i = 0; i < file_number; i++)
    {
        sprintf(filename, "%s/Frame%d.jpg", dir_name, i);
        if(read_JPEG_file(filename, lcd_info->lcd_mp, start_x, start_y) < 0)
        {
            perror("read_JPEG_file error");
            return -1;
        }
        memset(filename, 0, sizeof(filename));
        usleep(speed * 1000);

    }
    return 0;
}

/**********************************
 * 
 *          图片压缩         
 * 
 * ********************************/
/*********************************************
 *  函数功能: 压缩图片
 *  输入参数:   @lcd_info: lcd_info_t结构体
 *              @picture_path: 图片的路径
 *              @rate: 压缩率，1为原来的图片，2为缩小2倍，3为缩小3倍，4为缩小4倍
 *              @start_x: 图片开始显示的x轴坐标
 *              @start_y: 图片开始显示的y轴坐标
 *  函数返回值: 0表示成功，-1表示失败
 *  说明: 图片缩小为4倍后，其效果不怎么好，因此，缩小倍率最好为4倍以下
 * ******************************************/
int show_picture(lcd_info_t *lcd_info ,const char *picture_path, int rate,
                    int start_x, int start_y)
{
    /* 参数有效性 */
    if (picture_path == NULL || start_x < 0 || start_y < 0 
        || start_x >= 800 || start_y >= 480 || lcd_info == NULL)
    {
        perror("intput argument error");
        return -1;
    }
    /* 对上述函数进行改写 */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* More stuff */
    FILE * infile;		/* source file */
    unsigned char* buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */
    if(lcd_info->lcd_mp == NULL)
    {
        perror("lcd mmap error");
        return -1;
    }
    if ((infile = fopen(picture_path, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", picture_path);
    return 0;
    }

    /* Step 1: allocate and initialize JPEG decompression object */
    /* Now we can initialize the JPEG decompression object. */
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */

    jpeg_stdio_src(&cinfo, infile);

    /* Step 3: read file parameters with jpeg_read_header() */

    jpeg_read_header(&cinfo, TRUE);

    /* Step 4: set parameters for decompression */

    /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */

    /* Step 5: Start decompressor */

    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (unsigned char *)calloc(1, row_stride);
    if(buffer == NULL)
    {
        perror("can not calloc");
        return -1;
    }

    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */

    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    /* 备份图片的宽和高 */
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int p = 0;       /* 列压缩 */   
    while ((cinfo.output_scanline) < (height)) 
    {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
            * Here the array is only one element long, but you could ask for
            * more than one scanline at a time if that's more convenient.
            */
        /*  
            压缩图片，1:1压缩
        */
        int j = 0;       /* 行压缩 */
        
        for(int i = 0; i < rate; i++)
            jpeg_read_scanlines(&cinfo, &buffer, 1);
        
        for(int i = 0; i < (width); i++, j+=rate)       
        {
            *(lcd_info->lcd_mp + start_x + start_y * 800 + i 
                + (cinfo.output_scanline - (1 * rate) - p) * 800)   /* 对像素点进行换行 */
                =  (buffer[j * 3] << 16) | (buffer[j * 3 + 1] << 8) 
                        | buffer[j * 3 + 2]; // LCD -- ARGB   JPG --RGB 
        }
        p+=(rate - 1);
    }
    /* Step 7: Finish decompression */

    (void) jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* Step 8: Release JPEG decompression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

    fclose(infile);

    return 1;

}

