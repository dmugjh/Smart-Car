#ifndef __INCLUDE_H__
#define __INCLUDE_H__


#include  "common.h"

/*
 * Include 用户自定义的头文件
 */

#include  "MK60_gpio.h"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_adc.h"      //ADC
#include  "MK60_it.h"

#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY


#include "IIC_Analog.h"
#include  "control.h"
#include  "OLED.h"
#include  "Service.h"  
#include  "MPU9250.h"
#include "serial_gyro.h"

/*———————————参数宏定义—————————————*/

//编码器
#define Left_Pulse_Pin       PTA12
#define Left_Direction_Pin   PTA13
#define Right_Pulse_Pin      PTC5
#define Right_Direction_Pin  PTA19
#define INT_COUNT            0xFFFF          //LPT 产生中断的计数次数

#define KEY_A PTB18
#define KEY_B PTB20
#define KEY_C PTB21
#define KEY_D PTB22
#define KEY_E PTB23       //按键接口的初始化

#define SWITCH_A PTA7
#define SWITCH_B PTA8
#define SWITCH_C PTA9
#define SWITCH_D PTA10

#define AD_CH0  ADC1_SE14
#define AD_CH1  ADC1_SE15
#define AD_CH2  ADC0_SE8
#define AD_CH3  ADC0_SE9
#define AD_CH4  ADC1_SE10

#endif  //__INCLUDE_H__
