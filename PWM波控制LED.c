#include <reg52.h>

typedef unsigned int u16;	//对系统默认数据类型进行重定义
typedef unsigned char u8;

sbit LED=P1^0;

void Delay(u16 t)
{
	while(t--);
}

void main()
{
	u8 Time,i;
	while(1)
	{
		for(Time=0;Time<100;Time++)
		{
			for(i=0;i<20;i++)
			{
				LED=0;
				Delay(Time);
				LED=1;
				Delay(100-Time);
			}
		}
		for(Time=100;Time>0;Time--)
		{
			for(i=0;i<20;i++)
			{
				LED=0;
				Delay(Time);
				LED=1;
				Delay(100-Time);
			}
		}
	}
}
