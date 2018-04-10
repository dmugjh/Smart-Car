#include "include.h"
void init()
{
   #if  MK60F15 
   SCB->CPACR |=((3UL << 10*2)|(3UL << 11*2));     /* set CP10 and CP11 Full Access */
   #endif               //开启硬件浮点运算
/*———————————中断向量优先级配置——————————————*/
                NVIC_SetPriorityGrouping(0);                               //配置中断优先级分组
                NVIC_SetPriority(PORTA_VECTORn,0);                         //场终端，必须是最高优先级，其它无所谓
                NVIC_SetPriority(PIT0_VECTORn, 1);                         //定时中断PIT0,实现控制周期
                NVIC_SetPriority(FTM1_VECTORn, 2);                         //输入捕捉中断用来测一路速度
                NVIC_SetPriority(UART4_RX_TX_VECTORn, 0); 
                gpio_init (PTD15,GPO, 0);
  /*———————————蜂鸣器初始化—————————————*/
                gpio_init (PTA16,GPO, 0);
                gpio_init (PTE26,GPO, 1);
  /*———————————按键初始化———————————*/
                gpio_init (KEY_A,GPI,0);
                gpio_init (KEY_B,GPI,0);
                gpio_init (KEY_C,GPI,0);
                gpio_init (KEY_D,GPI,0);
                gpio_init (KEY_E,GPI,0);
/*———————————拨码开关初始化———————————*/
                gpio_init (SWITCH_A,GPI,0);
                gpio_init (SWITCH_B,GPI,0);
                gpio_init (SWITCH_C,GPI,0);
                gpio_init (SWITCH_D,GPI,0);      

  /*————————————LED显示屏初始化——————————————*/
                        LED_Init();
  /*————————————左电机测速初始化—————————————*/
                ftm_input_init(FTM1,FTM_CH0,FTM_Falling,FTM_PS_1);            
                port_init_NoALT(Left_Pulse_Pin ,PULLUP);                       
                set_vector_handler(FTM1_VECTORn ,FTM1_INPUT_IRQHandler);
                enable_irq(FTM1_IRQn);
                gpio_init(Left_Direction_Pin,GPI,0);                                      
  /*——————————右电机测速初始化————————————*/
                lptmr_pulse_init(LPT0_ALT2,0xFFFF,LPT_Rising);              
                gpio_init(Right_Direction_Pin,GPI,0);                        
  /*————————————电机初始化——————————————*/
                ftm_pwm_init(FTM0, FTM_CH0,8000, 0);//右反 
                ftm_pwm_init(FTM0, FTM_CH1,8000, 0);//右前
                ftm_pwm_init(FTM0, FTM_CH2,8000, 0);//左前
                ftm_pwm_init(FTM0, FTM_CH3,8000, 0);//左反 
  /*————————————舵机电机初始化————————————*/
                adc_init(AD_CH0);
                adc_init(AD_CH1);
                adc_init(AD_CH2);
                adc_init(AD_CH3);
                ftm_pwm_init(FTM2, FTM_CH1,100, 1400);                        
  /*————————————uart初始化——————————————*/
                uart_init(UART4,115200);
                set_vector_handler(UART4_RX_TX_VECTORn,uart4_test_handler);   
                uart_rx_irq_en(UART4);
                uart_init(UART0,9600);
                set_vector_handler(UART0_RX_TX_VECTORn,uart0_test_handler);   
                uart_rx_irq_en(UART0);
               // uart_init(UART1,9600);
  /*——————————PIT定时器初始化（CCD、测速、控速用）————————*/
                pit_init_ms(PIT0,8);                                  //初始化PIT0模块，设置1ms进一次中断
                set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);     //将中断服务函数加到中断向量表里
                enable_irq(PIT0_IRQn);                                //允许PIT0中断
  /*——————————I2C驱动以及MPU9250的初始化——————————————*/
                IIC_Init();//模拟I2C驱动的初始化，一定要先初始化I2C接口才能操作MPU9250芯片
                Init_MPU9250_With_DMP(); 

}




extern float Distance_ht,Distance_hr,Distance_tr,Angle_of_Target; 
extern float Yaw,Yaw_Ref; 
extern int16 Rear_x ,Rear_y ,Token_x , Token_y, Head_x, Head_y;
extern uint8 Key_Val_1,Key_Val_2,Key_Val_3,Key_Val_4,LED_Show_Flag;   //存储键值的变量(标志位)
extern float AngleError; 
extern struct SAngle stcAngle;
extern int16 Left_Speed,Right_Speed,Sys_Count;
extern int16 L_Set_Speed,R_Set_Speed;
void main(void)   
{
    init();         //初始化配置
    while(1)
    {

        MPU9250_search();
        Key_Scan();
        if(Key_Val_1 == 1)           //显示重要的偏角信息
        {
          LED_P6x8Str(0,0,"Yaw"); 
          LED_Show_Flag = 1;
          LED_PrintValueI(60,1,(int)Yaw);      
          LED_P6x8Str(0,2,"Angle_TC");   
          LED_PrintValueI(60,3,(int)Angle_of_Target);
          LED_P6x8Str(0,4,"AngleError:");          
          LED_PrintValueI(60,5,(int)AngleError);
        }
        else if(Key_Val_2 == 1)   //显示 有关AD采样值的信息
        {
          LED_Show_Flag = 1;                    //8.396*10^(-5)*adc_once(BIG_EYE0, ADC_10bit)^2 -0.1758*adc_once(BIG_EYE0, ADC_10bit) + 110.3   AD距离转换公式
          LED_PrintValueI(60,2,AD_To_Distance(AD_CH1, ADC_10bit));   
          LED_PrintValueI(60,4,AD_To_Distance(AD_CH2, ADC_10bit));  
          LED_PrintValueI(60,6,AD_To_Distance(AD_CH3, ADC_10bit)); 

          
        }
        else if(Key_Val_3 == 1)  //显示采集到的关键坐标点的信息
        {
          LED_Show_Flag = 1;
        
          LED_P6x8Str(0,1,"Rear:");
          LED_PrintValueI(0,2,(int)Rear_x);   LED_PrintValueI(50,2,(int)Rear_y);
          LED_P6x8Str(0,3,"Token:");
          LED_PrintValueI(0,4,(int)Token_x);   LED_PrintValueI(50,4,(int)Token_y);
          LED_P6x8Str(0,5,"Head:");
          LED_PrintValueI(0,6,(int)Head_x);   LED_PrintValueI(50,6,(int)Head_y);
        }
        else if(Key_Val_4 == 1)   //显示测定距离的信息
        {
          LED_Show_Flag = 1;
          LED_P6x8Str(0,0,"Distance_tr");
          LED_P6x8Str(0,2,"Distance_hr");
          LED_P6x8Str(0,4,"Distance_ht");
          LED_PrintValueI(60,1,(int)Distance_tr);
          LED_PrintValueI(60,3,(int)Distance_hr);       //这里的车首是假想出来的 不是真实的车首
          LED_PrintValueI(60,5,(int)Distance_ht);
          LED_P6x8Str(0,6,"Distance ^2:");
          LED_PrintValueI(60,7,(Rear_x-Token_x)^2 + (Rear_y - Token_y)^2);
        }
        else if( LED_Show_Flag == 1)
        {
          LED_CLS();     //清屏函数
          LED_Show_Flag = 0;
        }
     // Stevos_Test();           //舵机测试函数,调用时屏蔽其它函数
    }
}
