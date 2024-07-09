#include <reg52.h>    // 包含单片机寄存器定义头文件
#include <intrins.h>  // 包含一些内联汇编函数的头文件
#include <stdio.h>    // 标准输入输出头文件
#include <string.h>   // 字符串处理头文件

#define uchar unsigned char   // 定义无符号字符类型
#define uint unsigned int     // 定义无符号整型

sbit RS = P1^1;    // 将P1口的第1位定义为RS引脚
sbit EN = P1^0;    // 将P1口的第0位定义为EN引脚
sbit RW = P1^2;    // 将P1口的第2位定义为RW引脚
uchar code table1[] = {"hello,kevbin!"};  // 定义只读存储区的字符串table1
uchar code table2[] = {"0123456789"};    // 定义只读存储区的字符串table2

void delay(uint time);            // 延时函数声明
void init_1602(void);             // LCD初始化函数声明
void writecmd(uint cmd);          // 写指令函数声明
void writedata(uchar dat);        // 写数据函数声明
void display(uchar len, uchar* table);  // 显示字符串函数声明

 
void main(void)
{
    // 主函数
    delay(10);           // 延时10ms
    init_1602();         // 初始化LCD
    delay(5);            // 延时5ms
    writecmd(0x80);      // 设置显示位置为第一行第一个字符位置
    //delay(5);
    display(strlen(table1), table1);  // 显示table1字符串内容
    writecmd(0x80 + 0x40);            // 设置显示位置为第二行第一个字符位置
    display(strlen(table2), table2);  // 显示table2字符串内容
    while (1);  // 进入死循环
}

 
void delay(uint time)    // 自定义延时函数，参数为延时时间（毫秒）
{
    uint i, j; // 定义无符号整型变量i和j

    for(i = 0; i < time; i++) // 循环延时time次
    {
        for(j = 0; j < 11059; j++) // 内层循环，根据实际频率调整
        {
            _nop_(); // 执行空操作，延时一个机器周期
        }
    }
}

 
void init_1602(void)
{
    delay(15);       // 等待15ms
    //writecmd(0x33);  // 写指令
    //writecmd(0x32);  // 写指令
    writecmd(0x38);   // 设置工作模式// 16*2显示，5*7点阵，8位数据口
    writecmd(0x0c);   // 显示设置// 开显示，不显示光标
    writecmd(0x06);   // 设置显示模式// 地址加1，当写入数据后光标右移
    writecmd(0x01);   // 清屏
}

 
void writecmd(uint cmd) // 定义写命令函数，参数为一个无符号整型变量
{
    RS = 0;         // 设置RS为0，选择指令寄存器
    RW = 0;         // 设置RW为0，选择写入模式
    P0 = cmd;       // 将命令写入P0端口
    EN = 1;         // 使能端口为高电平，发生上升沿
    delay(1);       // 延时1毫秒
    EN = 0;         // 使能端口为低电平，发生下降沿
    /*
    P1 = (cmd & 0x0f) << 4; // 将命令的低4位移位并写入P1端口
    EN = 1;         // 使能端口为高电平，发生上升沿
    delay(1);       // 延时1毫秒
    EN = 0;         // 使能端口为低电平，发生下降沿
    */
}


 
void writedata(uchar dat) // 定义写数据函数，参数为一个无符号字符型变量
{
  RS = 1; // 设置RS为1，选择数据寄存器
  RW = 0; // 设置RW为0，选择写入模式
  P0 = dat; // 将数据写入P0端口
  EN = 1; // 使能端口为高电平，发生上升沿
  delay(1); // 延时1毫秒
  EN = 0; // 使能端口为低电平，发生下降沿
	/*
	P1 = (dat & 0x0f) << 4; // 将数据的低4位移位并写入P1端口
  EN = 1; // 使能端口为高电平，发生上升沿
  delay(1); // 延时1毫秒
  EN = 0; // 使能端口为低电平，发生下降沿
	*/
}

 
void display(uchar len, uchar* table) // 显示函数，参数为显示长度和显示内容数组指针
{
    uchar i; // 定义循环计数器

    for(i = 0; i < len; i++) // 循环显示每个字符
    {
        writedata(table[i]); // 调用写数据函数，将每个字符显示到液晶屏上
    }

    delay(5); // 延时5毫秒
}
