#include<reg52.h> // 包括 8052 微控制器的寄存器定义
#include<intrins.h> // 包括 8052 微控制器的固有函数
#define uchar unsigned char // 将 uchar 定义为无符号字符类型
#define uint unsigned int // 将 uint 定义为无符号整数类型

void xianwd(t1,t2,t3); // 包含三个参数 t1、t2 和 t3 的 xianwd 函数的函数原型

/******1302*********/ // 表示 DS1302 相关代码开始的注释
sbit rst=P1^5; // 将 rst 定义为端口 P1 上的一位，即第 5 位，用于 DS1302 复位
sbit io=P1^6; // 将 io 定义为端口 P1 第 6 位上的一位，用于 DS1302 数据输入/输出
sbit sclk=P1^7; // 将 sclk 定义为端口 P1 第 7 位上的单个位，用于 DS1302 时钟

/******1602*******/ // 注释表示 1602 LCD 相关代码的开始
sbit RS = P1^1; // 将 RS 定义为端口 P1 第 1 位上的一位，用于 1602 LCD 寄存器选择
sbit E = P1^0; // 将 E 定义为端口 P1 的单个位，位 0，用于 1602 LCD 使能

const uchar zhe1[]={'2','0','2','3','-','0','1','-','0','1',' ',' ',' ',' ',' ',' '};
// 定义一个常量字符数组 zhe1，存储字符串 "2023-01-01"
const uchar zhe2[]={'0','0',':','0','0',':','0','0',0x20,0x20,'K','e','v','b','i','n'}; 
// 定义一个常量字符数组 zhe2，用于存储字符串 "00:00:00 KeVBin"。

uchar time[7]={24,3,7,10,19,58,00}; // 定义一个由 7 个无符号字符组成的数组，用于存储时间信息，年、周、月、日、时、分、秒
uchar write_add[]={0x8c,0x8a,0x88,0x86,0x84,0x82,0x80}; // 为 DS1302 定义一个写地址数组
uchar read_add[]={0x8d,0x8b,0x89,0x87,0x85,0x83,0x81}; // 为 DS1302 定义读地址数组
uchar n1,n2,y1,y2,r1,r2,s1,s2,f1,f2,m1,m2,xq; // 定义各种单字节变量
uchar i; // 定义一个单字节变量 i

void delay(unsigned int k) {
    // 延迟函数，输入为无符号整数 k
    unsigned char a, b;
    for (a = k; k > 0; k--) { // 外循环，迭代 k 次。
        // 外循环，遍历 k 次
        for (b = 110; b > 0; b--) { // 内循环，迭代 110 次
            // 内循环，迭代 110 次，产生延迟
        }
    }
}

void cz(uchar cz) { // 发送操作指令
    // cz 函数，以无符号字符 cz 作为输入
    RS = 0; // 将 RS（寄存器选择）设为 0，表示指令模式
    P0 = cz; // 通过端口 P0 向 LCD 控制器发送指令
    E = 0; // 将 E（使能）设为 0，准备发送指令
    delay(1); // 等待一小段时间
    E = 1; // 将 E（使能）设为 1，使指令得以发送
}

void sj(uchar sj) { // 发送 LCD 数据
    // sj 函数，以无符号字符 sj 作为输入
    RS = 1; // 将 RS（寄存器选择）置 1，表示数据模式
    P0 = sj; // 通过端口 P0 向 LCD 控制器发送数据
    E = 0; // 将 E（使能）设为 0，准备发送数据
    delay(1); // 等待一小段时间
    E = 1; // 将 E（使能）设为 1，使数据能够发送
}

void xiannyr(uchar n1, uchar n2, uchar y1, uchar y2, uchar r1, uchar r2)
{
    // xiannyr 函数，显示年、月、日
    // 接收 6 个无符号字符输入：n1, n2, y1, y2, r1, r2
    
    // 向 LCD 发送指令，将光标移动到年份显示位置（在 LCD 的第 0 行第 2 列）
    cz(0x82);
    
    // 向 LCD 发送年份十位数（ASCII 码 0-9 为 0x30-0x39）
    // 例如，如果 n1 = 2，则发送 ASCII 码 0x32，表示数字 2
    sj(0x30 + n1);
    
    // 向 LCD 发送年份单位数字
    // 例如，如果 n2 = 5，则发送 ASCII 码 0x35，表示数字 5
    sj(0x30 + n2);
    
    // 向 LCD 发送指令，将光标移至月份显示位置（在 LCD 的第 0 行第 5 列）
    cz(0x85);
    
    // 向 LCD 发送月份十位数指令
    // 例如，如果 y1 = 1，则发送 ASCII 码 0x31，表示数字 1
    sj(0x30 + y1);
    
    // 向 LCD 发送月份单位数字
    // 例如，如果 y2 = 2，则发送 ASCII 码 0x32，表示数字 2
    sj(0x30 + y2);
    
    // 向 LCD 发送指令，将光标移至日显示位置（在 LCD 的第 0 行第 8 列）
    cz(0x88);
    
    // 向 LCD 发送星期十位数指令
    // 例如，如果 r1 = 2，则发送 ASCII 码 0x32，表示数字 2
    sj(0x30 + r1);
    
    // 向 LCD 发送日单位数字
    // 例如，如果 r2 = 8，则发送 ASCII 码 0x38，表示数字 8
    sj(0x30 + r2);
}


void xiansfm(uchar n1, uchar n2, uchar y1, uchar y2, uchar r1, uchar r2)
{
    // xiansfm 函数，显示小时、分钟和秒。
    // 接收 6 个无符号字符输入：n1、n2、y1、y2、r1、r2
    
    // 向 LCD 发送指令，将光标移动到小时显示位置（在 LCD 的第一行第四列）
    // 0x80 是 LCD 的第一行的基本地址，+ 0x40 是为了选择第二行
    cz(0x80 + 0x40);
    
    // 向 LCD 发送小时十位数指令
    // 例如，如果 n1 = 1，则发送 ASCII 码 0x31，表示数字 1
    sj(0x30 + n1);
    
    // 向 LCD 发送小时单位数字
    // 例如，如果 n2 = 2，则发送 ASCII 码 0x32，表示数字 2
    sj(0x30 + n2);
    
    // 向 LCD 发送指令，将光标移至分钟显示位置（在 LCD 的第一行第七列）
    cz(0x83 + 0x40);
    
    // 向 LCD 发送分钟十位数指令
    // 例如，如果 y1 = 4，则发送 ASCII 码 0x34，表示数字 4
    sj(0x30 + y1);
    
    // 向 LCD 发送分钟单位数字
    // 例如，如果 y2 = 5，则发送 ASCII 码 0x35，表示数字 5
    sj(0x30 + y2);
    
    // 向 LCD 发送指令，将光标移动到秒显示位置（在 LCD 的第一行第十列）
    cz(0x86 + 0x40);
    
    // 向 LCD 发送秒十位数指令
    // 例如，如果 r1 = 3，则发送 ASCII 码 0x33，表示数字 3
    sj(0x30 + r1);
    
    // 向 LCD 发送秒单位数字
    // 例如，如果 r2 = 9，则发送 ASCII 码 0x39，表示数字 9
    sj(0x30 + r2);
}

void xianxq(uchar n) // 显示星期
{
    cz(0x8D); // 清空显示屏
    
    // 根据星期几的值，显示对应的英文缩写
    if(n==0) // SUN
    {
        sj(0x53); // 显示 "S
        sj(0x75); // 显示 "U
        sj(0x6e); // 显示 "N
    }
    if(n==1) // MON
    {
        sj(0x4d); // 显示 "M
        sj(0x6f); // 显示 "O
        sj(0x6e); // 显示 "N
    }
    if(n==2) // TUE
    {
        sj(0x54); // 显示 "T
        sj(0x75); // 显示 "U
        sj(0x65); // 显示 "E
    }
    if(n==3) // WED
    {
        sj(0x57); // 显示'W' } if(n==3) // WED {
        sj(0x65); // 显示 "E
        sj(0x64); // 显示 "D
    }
    if(n==4) // THU
    {
        sj(0x54); // 显示'T
        sj(0x68); // 显示 "H
        sj(0x75); // 显示 "U
    }
    if(n==5) // FRI
    {
        sj(0x46); // 显示'F
        sj(0x72); // 显示 "R
        sj(0x69); // 显示 "I
    }
    if(n==6) // SAT
    {
        sj(0x53); // 显示'S
        sj(0x61); // 显示'A
        sj(0x74); // 显示 "T
    }
}
void csh() // 1602 LCD初始化
{
    // 发送命令：设置显示方式为8位接口、2行显示、5x7点阵
    cz(0x38);
    delay(5); // 等待命令执行完成
    
    // 发送命令：打开显示、关闭游标
    cz(0x0C);
    delay(5); // 等待命令执行完成
    
    // 发送命令：入射方式增量、右移
    cz(0x06);
    delay(5); // 等待命令执行完成
    
    // 发送命令：清空显示屏
    cz(0x01);
    delay(5); // 等待命令执行完成
}

/*********1302************/
/****写位*****/
void write_byte(uchar sj) // 写一个字节到串行总线
{
    uchar i;
    for(i=8; i>0; i--) // 循环8次，逐位发送字节
    {
        // 使能串行时钟信号，使其处于低电平状态
        sclk=0;
        
        // 将要发送的字节sj的最低位取出，发送到串行数据线io
        io=sj&0x01;
        
        // 将sj右移一位，准备发送下一位
        sj=sj>>1;
        
        // 释放串行时钟信号，使其处于高电平状态
        sclk=1;
    }
}

/****写数据*****/
void write(uchar add, uchar sj)
{
    // 将 RST 口置低电平，准备写入数据
    rst = 0;
    
    // inserting a NOP (no operation) cycle to ensure setup time
    _nop_();
    
    // 将 SCLK 口置低电平，准备写入数据
    sclk = 0;
    
    // inserting another NOP cycle to ensure setup time
    _nop_();
    
    // 将 RST 口置高电平，开始写入数据
    rst = 1;
    
    // inserting another NOP cycle to ensure hold time
    _nop_();
    
    // 写入地址 byte 到 LCD
    write_byte(add);
    
    // 写入数据 byte 到 LCD
    write_byte(sj);
    
    // 将 RST 口置低电平，写入完成
    rst = 0;
    
    // inserting another NOP cycle to ensure setup time
    _nop_();
    
    // 将 IO 口置高电平，结束写入
    io = 1;
    
    // 将 RST 口置高电平，结束写入
    rst = 1;
}


uchar read(uchar add) // 读取数据根据地址
{
    // 初始化变量 i 和 value
    uchar i, value = 0x00;
    
    // 将 RST 口置低电平，准备读取数据
    rst = 0;
    
    // inserting a NOP cycle to ensure setup time
    _nop_();
    
    // 将 SCLK 口置低电平，准备读取数据
    sclk = 0;
    
    // inserting another NOP cycle to ensure setup time
    _nop_();
    
    // 将 RST 口置高电平，开始读取数据
    rst = 1;
    
    // inserting another NOP cycle to ensure hold time
    _nop_();
    
    // 将地址 byte 发送到 LCD
    write_byte(add);
    
    // 循环读取 8位数据
    for (i = 0; i < 8; i++)
    {
        // 右移 value 变量 1 位，准备读取下一位
        value = value >> 1;
        
        // 将 SCLK 口置低电平，准备读取
        sclk = 0;
        
        // 读取当前位的数据，如果 IO 口为高电平，则将 value 变量左移一位并置高位
        if (io)
            value = value | 0x80;
        
        // 将 SCLK 口置高电平，完成读取
        sclk = 1;
    }
    
    // 将 RST 口置低电平，读取完成
    rst = 0;
    
    // inserting multiple NOP cycles to ensure setup and hold times
    _nop_();
    _nop_();
    rst = 0;
    sclk = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    sclk = 1;
    _nop_();
    _nop_();
    
    // 将 IO 口置低电平，准备读取完成
    io = 0;
    
    // inserting multiple NOP cycles to ensure setup and hold times
    _nop_();
    _nop_();
    
    // 将 IO 口置高电平，读取完成
    io = 1;
    
    // inserting multiple NOP cycles to ensure setup and hold times
    _nop_();
    _nop_();
    
    // 返回读取的数据值
    return (value);
}


void init_rtc() // 初始化 1302 RTC
{
    // 声明变量 i 和 j
    uchar i, j;
    
    // 将 time 数组中的 BCD 码转换为十六进制
    for (i = 0; i < 7; i++)
    {
        // 将当前时间值除以 10，得到十位数字
        j = time[i] / 10;
        
        // 将当前时间值对 10 求余，得到个位数字
        time[i] = time[i] % 10;
        
        // 将十位数字左移 4位（乘以 16），然后加上个位数字，得到十六进制表示的时间值
        time[i] = time[i] + j * 16;
    }
    
    // 去除写保护，写入控制字 0x00 到寄存器 0x8e
    write(0x8e, 0x00);
    
    // 将时间值写入 RTC
    for (i = 0; i < 7; i++)
    {
        // 将当前时间值写入到对应的寄存器
        write(write_add[i], time[i]);
    }
    
    // 写保护，加上控制字 0x80 到寄存器 0x8e
    write(0x8e, 0x80);
}

void read_rtc() // 读取时间数值
{
    // 声明变量 i 和指针 p
    uchar i, *p;
    
    // 将指针 p 指向 read_add 数组
    p = read_add;
    
    // 读取时间值
    for (i = 0; i < 7; i++)
    {
        // 读取当前时间值，并存储到 time 数组
        time[i] = read(*p);
        
        // 指针 p 向前移动一个位置
        p++;
    }
}

void time_pors()//取时间值
{
    // 提取秒（second）值
    m1=time[6]%16;// 副位（低4位），即秒的个位
    m2=time[6]/16;// 主位（高4位），即秒的十位
    
    // 提取分（minute）值
    f1=time[5]%16;// 副位（低4位），即分的个位
    f2=time[5]/16;// 主位（高4位），即分的十位
    
    // 提取时（hour）值
    s1=time[4]%16;// 副位（低4位），即时的个位
    s2=time[4]/16;// 主位（高4位），即时的十位
    
    // 提取日（day）值
    r1=time[3]%16;// 副位（低4位），即日的个位
    r2=time[3]/16;// 主位（高4位），即日的十位
    
    // 提取月（month）值
    y1=time[2]%16;// 副位（低4位），即月的个位
    y2=time[2]/16;// 主位（高4位），即月的十位
    
    // 提取年（year）值
    n1=time[0]%16;// 副位（低4位），即年的个位
    n2=time[0]/16;// 主位（高4位），即年的十位
    
    // 提取星期（week）值
    xq=time[1]%16;// 副位（低4位），即星期的个位
}

void main()
{
    // 声明变量 a，用于循环和计数用
    uchar a;
    
    // 清除显示屏
    csh();
    
    // 设置 1602 的第一个显示位，地址为 0x80
    cz(0x80);
    
    // 初始化显示第一行 1602 数值
    for(a=0;a<16;a++)
    {
        // 发送字符到 1602 显示模块
        sj(zhe1[a]);
        
        // 等待 5ms，以避免显示闪烁
        delay(5);
    }
    
    // 换行，设置 1602 的第二个显示位，地址为 0xC0
    cz(0xC0);
    
    // 显示 1602 第二行数值
    for(a=0;a<16;a++)
    {
        // 发送字符到 1602 显示模块
        sj(zhe2[a]);
        
        // 等待 5ms，以避免显示闪烁
        delay(5);
    }
    
    // 初始化 1302 RTC 模块
    init_rtc();
    
    // 主循环
    while(1)
    {
        // 读取 1302 RTC 模块的时间值
        read_rtc();
        
        // 取出 1302RTC 的时间值，并处理成适合显示的格式
        time_pors();
        
        // 调用键盘事件，用于调整时间
        
        // 显示年月日
        xiannyr(n2,n1,y2,y1,r2,r1);
        
        // 显示时分秒
        xiansfm(s2,s1,f2,f1,m2,m1);
        
        // 显示星期
        xianxq(xq);
    }
}

