/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,山外科技
*     All rights reserved.
*     技术讨论：山外论坛 http://www.vcan123.com
*
*     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
*     修改内容时必须保留山外科技的版权声明。
*
* @file       MK60_it.c
* @brief      山外K60 平台中断服务函数
* @author     山外科技
* @version    v5.0
* @date       2013-06-26
*/
#include  "include.h"
#include  "MK60_it.h"
/*-----------------左侧电机------------------------------*/
int16 cesu_left_count=0;        //左侧电机计数
int16 Left_Speed=0;         //描述当前速度的变量，可以有正负，其实就是编码器在10ms内测得的脉冲计数值
/*-----------------右侧电机------------------------------*/
//右侧电机
uint8 Right_Direction=1;
int16 Right_Speed=0;         //描述当前速度的变量，可以有正负，其实就是编码器在10ms内测得的脉冲计数值
/*—————————测速用到的变量—————————————*/
int flag_cesutime=0;          //测速任务分时，进程管理器
int flag_gengxinsudu=0;
int flag_dianjicontrol = 0;


uint32 left_tyre_count=0;    //左侧车轮的脉冲计数
uint32 left_tyre_dir,right_tyre_dir;   //左右侧车轮的转动方向

void FTM1_INPUT_IRQHandler(void)    //第一路测速
{
    uint8 s = FTM1_STATUS;          //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;

    FTM1_STATUS = 0x00;             //清中断标志位

    CHn = 0;
    if( s & (1 << CHn) )
    {
        left_tyre_count++;             //速度脉冲累加
    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
}


/*!
 *  @brief      PIT中断服务函数
 *  @since      v5.0
 */
int16 Left_Speed,Right_Speed,Start_Count=0,Sys_Count = 0;         //电机的速度，便于外部的引用
extern int16 Rear_x ,Rear_y ,Token_x , Token_y ;
extern int8 Stop_Flag,Recov_Flag1,Recov_Flag2,icount;
void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0);  //清中断标志位，防止再次进入中断 
    int16 Left_Dir=0,Right_Dir=0;     //电机转动的方向 
    Left_Dir=gpio_get(Left_Direction_Pin);
    Right_Dir=gpio_get(Right_Direction_Pin);
    
    Left_Speed=(2*Left_Dir-1)*left_tyre_count;
    Right_Speed=(1-2*Right_Dir)*lptmr_pulse_get();  //速度脉冲累加的计算
   
    left_tyre_count=0;
    lptmr_pulse_clean();  //清空lptmr计数
    
    Start_Count++;
    Sys_Count ++;
    if(Sys_Count >=100) 
    {
      Stop_Flag = 1;             //超过 800 ms 串行口未接收到数据，强制停车
      Sys_Count = 100;
    }
    
    if(Start_Count>=200&& Rear_x !=0 && Token_x !=0)
    {
      Start_Count = 200;
       Speed_Control(); 
    //  Speed_Control_Test();
    }                           //通电一段时间后发车
     Servo_Control();
      
}



int8 Data_get, Data_buff[5] ,Recv_Msk_Flag = 0;

int16 Data_counter = 0 , ncount = 0;  
int16 Blue_pos_x1 = 0,Blue_pos_y1 = 0,Blue_pos_x2 = 0,Blue_pos_y2 = 0;     //记录蓝灯横纵坐标的变量
int16 Rear_x = 0,Rear_y = 0,Token_x = 0,Token_y = 0, Head_x = 0, Head_y = 0;
extern float Yaw;
void get_position(void)          //串行口中断接收一次完整数据的时间 42949us
{
  uart_getchar (UART4,&Data_get);//蓝牙接收
  Data_buff[Data_counter] = Data_get; 
  Sys_Count = 0;
  if(Data_counter == 0 && Data_buff[0] == 'E')
  {
    Stop_Flag = 1;     //接收到字符‘E’后强制停车
    Recov_Flag1 = 0;
    Recov_Flag2 = 0;
  }
  
//  if(Data_counter == 0 && Data_buff[0] == 'T')    icount ++;
 
  if(Data_counter == 0 && Data_buff[0] != 'A' && Data_buff[0] != 'B'&&Data_buff[0] != 'F'
              &&Data_buff[0] != 'G')
    return;
  Data_counter++;
  if(Data_counter == 4)
  {
      Data_counter= 0;
      if(Data_buff[0] == 'A')           //以‘A B C D’开头的数据表示两个蓝灯的横纵坐标
      {
        Rear_x = atoi(&Data_buff[1]);
        Recov_Flag1 = 1;
      }
      else if(Data_buff[0] == 'B')
      {
        Rear_y = atoi(&Data_buff[1]);
      }
      else if(Data_buff[0] == 'F')
      {
        Token_x = atoi(&Data_buff[1]); 
        Recov_Flag2 = 1;
      }
      else if(Data_buff[0] == 'G')
      {
        Token_y= atoi(&Data_buff[1]);
      } 
   }
  
}
  
void uart4_test_handler()
{
  UARTn_e uratn = UART4;
  if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
  {
    get_position();

  }
        
}

int8 Recv_Data = 0;
void uart0_test_handler()
{
  UARTn_e uratn = UART0;
  if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
  {
    uart_getchar (UART0,&Recv_Data);       //串口接收
    CopeSerialData(Recv_Data);
  }
        
}

