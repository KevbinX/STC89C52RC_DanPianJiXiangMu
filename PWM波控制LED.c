#include <reg52.h>  // 包含reg52.h头文件，该文件定义了常用的单片机寄存器和指令等

typedef unsigned int u16;  // 定义无符号整型变量u16，用于存储16位无符号整数
typedef unsigned char u8;   // 定义无符号字符变量u8，用于存储8位无符号整数

sbit LED=P1^0;  // 定义LED引脚，连接到P1的第0位引脚

void Delay(u16 t)
{
    while(t--);  // 延时函数，通过循环实现延时
}

void main()
{
    u8 Time,i;  // 定义变量Time和i，用于循环计数

    while(1)  // 进入无限循环
    {
        for(Time=0;Time<100;Time++)  // 第一个循环，控制LED亮度逐渐增加
        {
            for(i=0;i<20;i++)  // 内部循环，控制LED闪烁次数
            {
                LED=0;  // 点亮LED
                Delay(Time);  // 延时Time个单位
                LED=1;  // 熄灭LED
                Delay(100-Time);  // 延时100-Time个单位
            }
        }

        for(Time=100;Time>0;Time--)  // 第二个循环，控制LED亮度逐渐减小
        {
            for(i=0;i<20;i++)  // 内部循环，控制LED闪烁次数
            {
                LED=0;  // 点亮LED
                Delay(Time);  // 延时Time个单位
                LED=1;  // 熄灭LED
                Delay(100-Time);  // 延时100-Time个单位
            }
        }
    }
}
