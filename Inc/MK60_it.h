
/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.h
 * @brief      山外K60 平台中断服务重定向头文件
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */


#ifndef __MK60_IT_H__
#define __MK60_IT_H__

void PORTA_IRQHandler();//摄像头行中断
void DMA0_IRQHandler();//摄像头场中断

void FTM1_INPUT_IRQHandler();//左侧电机测速
void PIT0_IRQHandler();
void MPU9250_search();
void uart4_test_handler();
void uart0_test_handler();


#endif  //__MK60_IT_H__
