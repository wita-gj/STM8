/* Copyright (c) 2022, wita_gj <wita_gj.com>
 * All rights reserved.
 *
 * stm8 流水灯,使用3-8译码器，
 * 其中G1，G2A,G2B均已接电源或接地
 * 使用PB端口低三位，连接A，B，C
 * IDE：IAR STM8
 * BOARD：MB867A
 * 3-8：SN74HC138N
 */

#include <iostm8s105c6.h>
//延时函数,非精确值
void delay(int time_ms)
{
  for(int i=0;i++<time_ms;)
  {;}
}
unsigned char tmp=0x00;          //计数
unsigned char dirction=0;        //流水方向,初始递增
void main( void )
{
  PB_DDR=0xFF;  //设置为输出模式
  PB_CR1=0xFF;  //设置为推挽输出
  PB_CR2=0x00;  //设置为慢速模式
  PB_ODR=0xF8;  //初始化,使用后三位
  
  //unsigned char tmp=0x00;     //计数值,可设置为局部变量，设置为全局时可使用IAR和查看值。
   /* 单向流水灯
    while (1)     //主循环
    {
      if (tmp>0x07)
      {
        tmp=0x00;
      }
      delay(20000);
      PB_ODR=(tmp++|0xF8);
    }
   */
  //循环双向流水灯，使用3-8译码器
  while (1)     //主循环
  {
    delay(20000);
    if (tmp==0x07)      
    {
      dirction=!dirction;      //递增方向换向
    }
    if(tmp==0xFF)
    {
      dirction=!dirction;       //递减方向换向
      tmp=tmp+2;                //溢出值修正
    }
    if(dirction>0)
    {
      PB_ODR=(tmp--|0xF8);      //减向
    }
    else
    {
       PB_ODR=(tmp++|0xF8);     //增向
    }
  }
}



