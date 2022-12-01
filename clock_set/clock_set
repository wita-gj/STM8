/* Copyright (c) 2022, wita_gj <wita_gj.com>
 * All rights reserved.
 * 时钟切换，调整系统时钟分频系数。
 * 闪烁频率随时钟设置不同，
 * 自定义时钟切换函数，自动手动切换，根据不同参数实现，不同方式，不同时钟源。
 * 自定义内部HSI时钟分频系数调整函数，调整分频值。
*/
#include "iostm8s105c6.h"

const unsigned char AUTO_SW=1;
const unsigned char MAN_SW=0;
const unsigned char HSE_CLK=0xB4;       //高速外部时钟
const unsigned char HSI_CLK=0xE1;       //高速内部时钟
const unsigned char LSI_CLK=0xD2;       //使用内部低速时钟时确保选项字配置
                                          //LSI_EN=1


void delay(long int a)
{
  for (long int i=0;i<a;i++)
  { ; }
}


//Clock auto switch
unsigned char clock_sw_auto(unsigned char clk_src)
{

  CLK_SWCR_SWEN=1;      //配置使能
  CLK_SWCR_SWIEN=0;     //禁止切换产生中断
  CLK_SWR=clk_src;         //外部高速时钟。
  delay(100);
  return CLK_CMSR;
}
//Clock manual switch
unsigned char clock_sw_man(unsigned char clk_src)
{
  CLK_SWCR_SWIEN=0;     //禁止切换产生中断
  CLK_SWR=clk_src;         //外部高速时钟。
  while (CLK_SWCR_SWIF==0)
  {
    delay(100);
  }
  CLK_SWCR_SWEN=1;      //配置使能
  return CLK_CMSR;
}

//Clock switch 
unsigned char clock_sw(unsigned char sw_method,unsigned char clk_src)
{
  unsigned char main_clk;
  if(sw_method)
  {
    //auto
    main_clk=clock_sw_auto(clk_src);
  }
  else
  {
    //manual
     main_clk=clock_sw_man(clk_src);
  }
  return main_clk;
}
//HSI_div modify,
//修改主时钟分频系数，根据参数
void mod_hsi_div(unsigned char clk_div)
{
    if (CLK_CSSR&0x08==0x08)
    {
      CLK_CSSR&=0xF7;
    }
  switch (clk_div)
  {
    case 1 :
    {
      CLK_CKDIVR&=0x07;
      break;
    }
    case 2 :
    {
      CLK_CKDIVR&=0x0F;
      break;
    }
    case 4 :
    {
       CLK_CKDIVR&=0x17;
       break;
    }
    case 8 :
    {
      CLK_CKDIVR&=0x1F;
      break;
    }
    default:
    {
      ;
    }
  }
}

int main( void )
{
  //unsigned char clock_info;
  PD_DDR_DDR0=1;       //输出
  PD_CR1_C10=1;       //输出
  PD_CR2_C20=0;         //慢速
  PD_ODR_ODR0=0;
  for (int i=0;i<5;i++)
  {
    PD_ODR_ODR0=~PD_ODR_ODR0;
    delay(40000);
  }
  //clock_info=clock_sw_auto(clk_src);  //自动切换主时钟
  //clock_info=clock_sw_man(clk_src);   //手动切换主时钟
  //clock_sw(AUTO_SW,LSI_CLK);          //根据参数切换主时钟
  //CLK_CKDIVR&=0x07;   //修改为1分频，最高频率。
  mod_hsi_div(1);
  for (int i=0;i<1;)
  {
    PD_ODR_ODR0=~PD_ODR_ODR0;
    delay(40000);
  }
  return 0;
}
