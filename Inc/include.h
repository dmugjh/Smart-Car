#ifndef __INCLUDE_H__
#define __INCLUDE_H__


#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */

#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
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

/*���������������������������궨�塪������������������������*/

//������
#define Left_Pulse_Pin       PTA12
#define Left_Direction_Pin   PTA13
#define Right_Pulse_Pin      PTC5
#define Right_Direction_Pin  PTA19
#define INT_COUNT            0xFFFF          //LPT �����жϵļ�������

#define KEY_A PTB18
#define KEY_B PTB20
#define KEY_C PTB21
#define KEY_D PTB22
#define KEY_E PTB23       //�����ӿڵĳ�ʼ��

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
