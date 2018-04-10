/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,ɽ��Ƽ�
*     All rights reserved.
*     �������ۣ�ɽ����̳ http://www.vcan123.com
*
*     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
*     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
*
* @file       MK60_it.c
* @brief      ɽ��K60 ƽ̨�жϷ�����
* @author     ɽ��Ƽ�
* @version    v5.0
* @date       2013-06-26
*/
#include  "include.h"
#include  "MK60_it.h"
/*-----------------�����------------------------------*/
int16 cesu_left_count=0;        //���������
int16 Left_Speed=0;         //������ǰ�ٶȵı�������������������ʵ���Ǳ�������10ms�ڲ�õ��������ֵ
/*-----------------�Ҳ���------------------------------*/
//�Ҳ���
uint8 Right_Direction=1;
int16 Right_Speed=0;         //������ǰ�ٶȵı�������������������ʵ���Ǳ�������10ms�ڲ�õ��������ֵ
/*�����������������������õ��ı�����������������������������*/
int flag_cesutime=0;          //���������ʱ�����̹�����
int flag_gengxinsudu=0;
int flag_dianjicontrol = 0;


uint32 left_tyre_count=0;    //��೵�ֵ��������
uint32 left_tyre_dir,right_tyre_dir;   //���Ҳ೵�ֵ�ת������

void FTM1_INPUT_IRQHandler(void)    //��һ·����
{
    uint8 s = FTM1_STATUS;          //��ȡ��׽�ͱȽ�״̬  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;

    FTM1_STATUS = 0x00;             //���жϱ�־λ

    CHn = 0;
    if( s & (1 << CHn) )
    {
        left_tyre_count++;             //�ٶ������ۼ�
    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
}


/*!
 *  @brief      PIT�жϷ�����
 *  @since      v5.0
 */
int16 Left_Speed,Right_Speed,Start_Count=0,Sys_Count = 0;         //������ٶȣ������ⲿ������
extern int16 Rear_x ,Rear_y ,Token_x , Token_y ;
extern int8 Stop_Flag,Recov_Flag1,Recov_Flag2,icount;
void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0);  //���жϱ�־λ����ֹ�ٴν����ж� 
    int16 Left_Dir=0,Right_Dir=0;     //���ת���ķ��� 
    Left_Dir=gpio_get(Left_Direction_Pin);
    Right_Dir=gpio_get(Right_Direction_Pin);
    
    Left_Speed=(2*Left_Dir-1)*left_tyre_count;
    Right_Speed=(1-2*Right_Dir)*lptmr_pulse_get();  //�ٶ������ۼӵļ���
   
    left_tyre_count=0;
    lptmr_pulse_clean();  //���lptmr����
    
    Start_Count++;
    Sys_Count ++;
    if(Sys_Count >=100) 
    {
      Stop_Flag = 1;             //���� 800 ms ���п�δ���յ����ݣ�ǿ��ͣ��
      Sys_Count = 100;
    }
    
    if(Start_Count>=200&& Rear_x !=0 && Token_x !=0)
    {
      Start_Count = 200;
       Speed_Control(); 
    //  Speed_Control_Test();
    }                           //ͨ��һ��ʱ��󷢳�
     Servo_Control();
      
}



int8 Data_get, Data_buff[5] ,Recv_Msk_Flag = 0;

int16 Data_counter = 0 , ncount = 0;  
int16 Blue_pos_x1 = 0,Blue_pos_y1 = 0,Blue_pos_x2 = 0,Blue_pos_y2 = 0;     //��¼���ƺ�������ı���
int16 Rear_x = 0,Rear_y = 0,Token_x = 0,Token_y = 0, Head_x = 0, Head_y = 0;
extern float Yaw;
void get_position(void)          //���п��жϽ���һ���������ݵ�ʱ�� 42949us
{
  uart_getchar (UART4,&Data_get);//��������
  Data_buff[Data_counter] = Data_get; 
  Sys_Count = 0;
  if(Data_counter == 0 && Data_buff[0] == 'E')
  {
    Stop_Flag = 1;     //���յ��ַ���E����ǿ��ͣ��
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
      if(Data_buff[0] == 'A')           //�ԡ�A B C D����ͷ�����ݱ�ʾ�������Ƶĺ�������
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
  if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
  {
    get_position();

  }
        
}

int8 Recv_Data = 0;
void uart0_test_handler()
{
  UARTn_e uratn = UART0;
  if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
  {
    uart_getchar (UART0,&Recv_Data);       //���ڽ���
    CopeSerialData(Recv_Data);
  }
        
}

