#include <reg52.H>  // 包含reg52单片机的头文件
#include <intrins.h>  // 包含位操作函数的头文件

sbit CE = P1^5;  // 定义片选引脚
sbit IO = P1^6;  // 定义数据输入输出引脚
sbit SCLK = P1^7;  // 定义时钟引脚
sbit LCD_RS = P1^1;  // 定义LCD的RS引脚
sbit LCD_RW = P1^2;  // 定义LCD的RW引脚
sbit LCD_EN = P1^0;  // 定义LCD的使能引脚

#define LCD_DataPort P0  // 定义LCD数据端口为P0
#define uchar unsigned char   // 定义无符号字符类型
#define uint unsigned int     // 定义无符号整型
	
unsigned char a[] = {0, 0, 0, 0, 0, 0};  // 定义一个包含6个元素的无符号字符数组a

void Delay100us(void)    //@11.0592MHz
{
    unsigned char i;  // 声明一个无符号字符型变量i

    _nop_();                // 空指令，延时一个机器周期
    i = 43;                 // 初始化i的值为43
    while (--i);            // 循环等待，实现大约100微秒延时
}


void LCD_WriteCommand(unsigned char Command)
{
    LCD_RS = 0;    // 将LCD_RS??脚置低，表示写入指令
    LCD_RW = 0;    // 将LCD_RW引脚置低，表示写入模式
    LCD_DataPort = Command;    // 将要写入的指令数据放入数据口
    LCD_EN = 1;    // 使能端置高，表示执行写入操作
    Delay100us();    // 延时100微秒
    LCD_EN = 0;    // 使能端置低，表示写入操作结束
    Delay100us();    // 延时时100微秒
}



void LCD_WriteData(unsigned char Data)
{
    LCD_RS = 1;    // 将LCD_RS引脚置高，表示写入数据
    LCD_RW = 0;    // 将LCD_RW引脚置低，表示写入模式
    LCD_DataPort = Data;    // 将要写入的数据放入数据口
    LCD_EN = 1;    // 使能端置高，表示执行写入操作
    Delay100us();    // 延时100微秒
    LCD_EN = 0;    // 使能端置低，表示写入操作结束
    Delay100us();    // 延时100微秒
}


void LCD_SetCursor(unsigned char Line, unsigned char Column)
{
    if (Line == 1)    // 如果要设置的行数为1
    {
        LCD_WriteCommand(0x80|(Column-1));    // 发送命令设置光标位置到第一行对应的列数
    }
    else if (Line == 2)    // 如果要设置的行数为2
    {
        LCD_WriteCommand(0x80|(Column-1+0x40));    // 发送命令设置光标位置到第二行对应的列数
    }
}


void LCD_Init()
{
    LCD_WriteCommand(0x38);  // 初始化液晶屏，设置为8位数据接口，2行显示，5x7点阵
    LCD_WriteCommand(0x0c);  // 开启显示，关闭光标，关闭闪烁
    LCD_WriteCommand(0x06);  // 写入数据后光标右移，不移动显示
    LCD_WriteCommand(0x01);  // 复位光标，清屏
}

int LCD_Pow(int X, int Y)
{
    unsigned char i;
    int Result = 1;    // 初始化结果为1
    for (i = 0; i < Y; i++)    // 循环Y次
    {
        Result *= X;    // 结果乘以X
    }
    return Result;    // 返回计算结果
}


void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length)
{
    unsigned char i;
    LCD_SetCursor(Line, Column);    // 设置光标位置
    for (i = Length; i > 0; i--)    // 从最高位开始显示数字
    {
        LCD_WriteData(Number / LCD_Pow(10, i-1) % 10 + '0');    // 将数字转换成字符并显示在液晶屏上
    }
}


void DS1302_write(unsigned char bite)
{
    unsigned char i;
    for (i = 0; i < 8; i++)    // 循环8次，写入一个字节的数据
    {
        SCLK = 0;    // 时钟信号置低
        IO = bite & (0x01 << i);    // 将当前位数据写入IO口
        SCLK = 1;    // 时钟信号置高
        _nop_();    // 空操作，延时
        SCLK = 0;    // 时钟信号置低
    }
}

void DS1302_writebite(unsigned char address, unsigned char dat)
{
    CE = 0;    // 片选信号置低
    _nop_();    // 空操作，延时
    SCLK = 0;    // 时钟信号置低
    _nop_();    // 空操作，延时
    CE = 1;    // 片选信号置高
    DS1302_write(address);    // 写入地址
    DS1302_write(dat);    // 写入数据
    CE = 0;    // 片选信号置低
}


unsigned char DS1302_readbite(unsigned char address)
{
    unsigned char i;
    unsigned char dat = 0x00;
    CE = 0;    // 片选信号置低
    _nop_();    // 空操作，延时
    SCLK = 0;    // 时钟信号置低
    _nop_();    // 空操作，延时
    CE = 1;    // 片选信号置高
    DS1302_write(address);    // 写入地址
    for (i = 0; i < 8; i++)
    {
        SCLK = 0;    // 时钟信号置低
        dat >>= 1;    // 右移一位
        if (IO)
        {
            dat |= 0x80;    // 如果IO口为高电平，设置dat的最高位为1
        }
        SCLK = 1;    // 时钟信号置高
        SCLK = 0;    // 时钟信号置低
    }
    CE = 0;    // 片选信号置低
    _nop_();    // 空操作，延时
    IO = 0;    // IO口置低
    _nop_();    // 空操作，延时
    dat = dat / 16 * 10 + dat % 16;    // 将BCD码转换为十进制数
    return dat;    // 返回读取到的数据
}


void DS1302_writetime()
{
    DS1302_writebite(0x8E, 0x00);    // 关闭写保护
    DS1302_writebite(0x80, 0x00);    // 秒数清0
    DS1302_writebite(0x82, 0x40);    // 分钟设置为 40
    DS1302_writebite(0x84, 0x15);    // 小时设置为 15
    DS1302_writebite(0x86, 0x09);    // 日设置为 9
    DS1302_writebite(0x88, 0x07);    // 月期设置为 7
    DS1302_writebite(0x8C, 0x24);    // 年设置为 24
    DS1302_writebite(0x8E, 0x80);    // 打开写保护
}


void DS1302_readtime()
{
    a[0] = DS1302_readbite(0x8D);    // 读取日期
    a[1] = DS1302_readbite(0x89);    // 读取月份
    a[2] = DS1302_readbite(0x87);    // 读取年
    a[3] = DS1302_readbite(0x85);    // 读取小时
    a[4] = DS1302_readbite(0x83);    // 读取分钟
    a[5] = DS1302_readbite(0x81);    // 读取秒数
}


void main()
{
    LCD_Init();         // 初始化LCD显示屏
    DS1302_writetime(); // 写入时间到DS1302实时时钟芯片

    while (1)
    {
        DS1302_readtime();          // 读取DS1302实时时钟芯片的时间数据
			Delay100us();
        LCD_ShowNum(1, 1, 20, 2);   // 在LCD显示屏上显示固定数字20
        LCD_ShowNum(1, 3, a[0], 2); // 在LCD显示屏上显示日期
        LCD_WriteData('-');         // 在LCD显示屏上显示短横线
        LCD_ShowNum(1, 6, a[1], 2); // 在LCD显示屏上显示月份
        LCD_WriteData('-');         // 在LCD显示屏上显示短横线
        LCD_ShowNum(1, 9, a[2], 2); // 在LCD显示屏上显示年
        LCD_ShowNum(2, 1, a[3], 2); // 在LCD显示屏上显示小时
        LCD_WriteData('-');         // 在LCD显示屏上显示短横线
        LCD_ShowNum(2, 4, a[4], 2); // 在LCD显示屏上显示分钟
        LCD_WriteData('-');         // 在LCD显示屏上显示短横线
        LCD_ShowNum(2, 7, a[5], 2); // 在LCD显示屏上显示秒数
    }
}
