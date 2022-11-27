/* Copyright (c) 2022, wita_gj <wita_gj.com>
 * All rights reserved.
 * EC12 Coder application
 */
/*
 * 由于STM8端口带负载能力强，直接将端口接VCC或接GND，其电流较大，
 * 使用0.1u电容难以滤除干扰，左右旋转可能带入反向干扰。
 * 端口使用10K上下拉电阻无反向干扰出现。
*/
#include "iostm8s105c6.h"

void delay(int a)
{
  for (int i=0;i<a;i++)
  { ; }
}

int rd=0;       //右转值
int ld=0;       //右转值
int rd_tmp;     //右转趋势值
int ld_tmp;     //左转趋势值
const int MAX_TURN=100; //调整最大值
unsigned char exit_cr2_v;

//EXIT4-Port E,绝对值，stm8手册中为偏移量
/*
#pragma vector=0x09
__interrupt void ec12(void)
{
  //一次拨码两次中断判断。
  //此种方式使用单引脚中断，判断另一引脚状态，有误码率，快速旋转时误码率高。
  //误码与稳定延时时长有关，与上下拉电阻及滤波电容大小有关。
  asm("sim");
  delay(100);
  if(PE_IDR_IDR0==0)
  {
    if (PE_IDR_IDR1==1)
    {
      rd_tmp=1;    //发生右转趋势
    } else{
      ld_tmp=1;    //发生左转趋势
    }
  } else {
     if ( PE_IDR_IDR1==0 && rd_tmp==1)
    {
      rd=rd<MAX_TURN ? ++rd : rd;    //右转
    }
    else if ( PE_IDR_IDR1==1 && ld_tmp==1)
    {
      ld=ld<MAX_TURN ? ++ld : ld;    //左转
    }
  }
  asm("rim");
}
*/

#pragma vector=0x09
__interrupt void ec12(void)
{
  //一次拨码一次中断，利用上升沿或下降沿判断。（此处下降沿）
  //此种方式使用单引脚中断，判断另一引脚状态，有误码率，快速旋转时误码率高。
  //误码与稳定延时时长有关，与上下拉电阻及滤波电容大小有关。
  asm("sim");
  delay(100);
  if (PE_IDR_IDR0==0 && PE_IDR_IDR1==1)
  {
    rd=rd+1;    //右转
  }
  else if (PE_IDR_IDR0==0 && PE_IDR_IDR1==0)
  {
    ld=ld+1;    //左转
  }
  asm("rim");
}

int main( void )
{
  
  PE_DDR&=0x00; //设置PE为输入模式
  PE_CR1=0x00;  //设置为悬浮输入，为防止部分IO无上拉电阻，
                //使用外部上拉信号，增强通用性
  PE_CR2=0x01;  //设置为开启中断，
  //设置PB为输出模式
  //PB_DDR&=0x07;
  //PB_CR1=0x07;
  //PB_CR2=0x00;
  /*
  1. 配置中断优先级，
  2. 配置触发类型，
  3. 开中断
    <-中断发生
  4. 中断服务后返回
  */
  //配置触发类型，下降沿触发
  //EXTI_CR1=0x00;
  EXTI_CR2|=0x02;     //下降沿触发
  //EXTI_CR2|=0x03;       //双沿触发
  //配置优先级
  ITC_SPR2=0xff;
  //调整main优先级，使能中断
  asm("rim");   //main 默认为3 优先级，执行后降低main为0优先级。间接实现中断。
  
  while(1)
  {
    ;
  }
  return 0;
}
