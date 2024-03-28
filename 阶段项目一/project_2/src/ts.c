#include "ts.h"

/* Copyright 12/8/2023 Szm */
/*
    函数名称：TS_Init
    函数功能：触摸屏的初始化
    函数参数：
               @tspath：在开发板上的ts的文件路径
    返回值：成功返回触摸屏的文件描述符， 失败返回-1
    函数作者：Szm
    函数版本：V1.0
    创建时间：02/08/2023
    注意事项：该触摸屏路径是在运行程序的硬件平台的路径
*/
int TS_Init(const char *tspath)
{
	//1.打开触摸屏并保存文件描述符   
	int ts_fd = open(tspath,O_RDWR);
	if(-1 == ts_fd)
	{
		perror("open touch screen error");
		return -1;
	}

	printf("touch screen init is OK!\n");
	return ts_fd;
}

/*
    函数名称：TS_GetVal
    函数功能：获取触摸点上的坐标
    函数参数：
               @tspath：在开发板上的ts的文件路径
               @ts_x：触摸点的x轴
               @ts_y：触摸点的y轴
    返回值：成功返回0
    函数作者：Szm
    函数版本：V1.0
    创建时间：02/08/2023
    注意事项：该触摸屏路径是在运行程序的硬件平台的路径
*/
int TS_GetVal(int ts_fd,int *ts_x,int *ts_y)
{
	//获取输入事件的信息并存储到指定类型的空间
	struct input_event ts_event;
	
	while(1)
	{
		read(ts_fd,&ts_event,sizeof(ts_event));

		//分析获取到的输入设备的参数信息：类型、编码、数值
		if (ts_event.type == EV_ABS && ts_event.code == ABS_X)
		{
			
			*ts_x = ts_event.value; //存储X轴坐标，该句需要选择性修改，新版触摸屏必须转换	
		}

		if (ts_event.type == EV_ABS && ts_event.code == ABS_Y)
		{
			
			*ts_y = ts_event.value; //存储Y轴坐标，该句需要选择性修改，新版触摸屏必须转换	
		}

		//当用户的手指离开触摸屏，则把坐标返回
		if(ts_event.type == EV_KEY && ts_event.code == BTN_TOUCH && ts_event.value == 0) 
		{
			printf("x=%d,y=%d\n",*ts_x,*ts_y);
			break;
		}
	}
	
	return 0;
}
