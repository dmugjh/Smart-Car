
/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.h
 * @brief      ɽ��K60 ƽ̨�жϷ����ض���ͷ�ļ�
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */


#ifndef __MK60_IT_H__
#define __MK60_IT_H__

void PORTA_IRQHandler();//����ͷ���ж�
void DMA0_IRQHandler();//����ͷ���ж�

void FTM1_INPUT_IRQHandler();//���������
void PIT0_IRQHandler();
void MPU9250_search();
void uart4_test_handler();
void uart0_test_handler();


#endif  //__MK60_IT_H__
