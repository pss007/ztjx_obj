#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "dht11.h" 	
#include <math.h>
#define PI   3.14159265358979323846
int32_t x1 = 1,y1 = 1,z1 = -1,x2 = -2,y2 = -2,z2 = 2,x3 = 1,y3 = -1,z3 = 2,x4 = 0,y4 = 0,z4 = 0,x5 = 0,y5 = 120,z5 = 0,x6,y6,z6;//已知条件，为常数，一共六个坐标
float z11,z22,z33;//偏移平面的三个点的z轴坐标
float init_x,init_y,init_z,offset_x,offset_y,offset_z;//参考点坐标和偏移后的坐标
float yaw,pitch,roll;
float temp1 = 7,temp2 = 7,temp3 = 7,temp4 = 5,temp5 = 5,temp6= 6;//六个传感器的测量数据
float A,B,C,D,A1,B1,C1,D1;//平面方程系数
float X,Y,Z;//偏移后参考点三个轴的变化量


typedef union test
{
    float a;
    u32 b;
}Test;

int  main(void)
{
volatile	float c,d;
	Test t;
	t.a = -1.234567;
	FLASH_Unlock();
	FLASH_ErasePage(0x8000000+2048*255);
	FLASH_ProgramWord(0x8000000+2048*255, t.b);
	FLASH_Lock();
	t.b = *(vu32*)(0x8000000+2048*255);
	c = t.a;
}
// int main(void)
// {	 
//	u8 t=0;			    
//   	   

//	delay_init();	    	 //延时函数初始化	  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//	uart_init(115200);	 	//串口初始化为115200
//	LED_Init();		  		//初始化与LED连接的硬件接口
//	LCD_Init();			   	//初始化LCD  
// 	POINT_COLOR=RED;		//设置字体为红色 
//	LCD_ShowString(230,50,200,16,24,"P S");
//	 
//	LCD_ShowString(120,80,300,16,24,"Pose analysis algorithm");	
//	 
//	POINT_COLOR=BLUE;//设置字体为蓝色  
//	 
//	LCD_ShowString(30,110,200,16,24,"X:            ");
//	LCD_ShowString(30,140,200,16,24,"Y:            ");		  						   
//	LCD_ShowString(30,170,200,16,24,"Z:            ");

// 	LCD_ShowString(30,200,200,16,24,"Yaw  :               ");	 
// 	LCD_ShowString(30,230,200,16,24,"Roll :               ");
// 	LCD_ShowString(30,260,200,16,24,"Pitch:               ");		
//	
//	A = (y2-y1)*(z3-z1)-(z2-z1)*(y3-y1);
//	B = (z2-z1)*(x3-x1)-(x2-x1)*(z3-z1);
//	C = (x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
//	D = x1*y3*z2+x2*y1*z3+x3*y2*z1-x1*y2*z3-x2*y3*z1-x3*y1*z2;
//	//接下来求出一个参考点的坐标，根据要求选两点4和6分别用到x轴 y轴坐标值，带入到平面方程求得z坐标值，至此，参考点坐标得到，代码如下
//	init_x = x4;
//	init_y = y6;
//	init_z = -(A*(init_x)+B*(init_y)+D)/C;
//	//至此参考点坐标已求出
//	//接下来求偏移后的平面方程，原三点x，y坐标保持不变，根据传感器位移，分别改变相应的z轴坐标，得到偏移平面上的三个点坐标，即可求出平面方程。
//	//新的三点坐标分别为（x1，y1，z1+temp1）（x2，y2，z2+temp2）（x3，y3，z3+temp3）
//	z11 = z1+temp1;
//	z22 = z2+temp2;
//	z33 = z3+temp3;
//	//由此可得，偏移平面的方程系数如下
//	A1 = (y2-y1)*(z33-z11)-(z22-z11)*(y3-y1);
//	B1 = (z22-z11)*(x3-x1)-(x2-x1)*(z33-z11);
//	C1 = (x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
//	D1 = x1*y3*z22+x2*y1*z33+x3*y2*z11-x1*y2*z33-x2*y3*z11-x3*y1*z22;
//	//至此 偏移的平面方程求出
//	//接下来求参考点偏移后的坐标值
//	offset_x = x4+temp4;
//	offset_y = y6+temp6;
//	offset_z = -(A1*offset_x-B1*offset_y+D1)/C1;// -(A1*(offset_x-x1)+B1*(offset_y-y1)+D1)/C1+z11;
//	//至此得到要求的X = temp4 ；Y = temp6；Z = offset_z-init_z；
//	X = temp4;
//	Y = temp6;
//	Z = offset_z-init_z;
//		
//	//4# 5#	航向角
//	yaw = atan(fabs(temp5-temp4)/(y5-y4))*180/PI;
//	//1# 2# 横滚角
//	roll = atan(fabs(temp1-temp2)/(x2-x1))*180/PI;
//	//1# 3# 俯仰角
//	pitch = atan(fabs((-(A1*((x2-x1)/2+x1)-B1*y1+D1)/C1)-temp3)/(y3-y1))*180/PI;
//		
//	POINT_COLOR=RED;		//设置字体为红色 
//	while(1)
//	{	    	    
//		if(t%10==0)			//每100ms读取一次
//		{									  			    
//			LCD_ShowNum(30+40,110,X,5,24);	//显示温度	   		   
//			LCD_ShowNum(30+40,140,Y,5,24);	//显示温度	   		   
//			LCD_ShowNum(30+40,170,Z,5,24);		//显示湿度	 
//			LCD_ShowNum(30+80,200,yaw,5,24);	//显示温度	   		   
//			LCD_ShowNum(30+80,230,pitch,5,24);	//显示温度	   		   
//			LCD_ShowNum(30+80,260,roll,5,24);		//显示湿度	 			
//		}				   
//	 	delay_ms(10);
//		t++;
//		if(t==20)
//		{
//			t=0;
//			LED0=!LED0;
//		}
//	}
//}




