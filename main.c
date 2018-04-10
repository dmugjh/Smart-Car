#include "include.h"
void init()
{
   #if  MK60F15 
   SCB->CPACR |=((3UL << 10*2)|(3UL << 11*2));     /* set CP10 and CP11 Full Access */
   #endif               //����Ӳ����������
/*�����������������������ж��������ȼ����á���������������������������*/
                NVIC_SetPriorityGrouping(0);                               //�����ж����ȼ�����
                NVIC_SetPriority(PORTA_VECTORn,0);                         //���նˣ�������������ȼ�����������ν
                NVIC_SetPriority(PIT0_VECTORn, 1);                         //��ʱ�ж�PIT0,ʵ�ֿ�������
                NVIC_SetPriority(FTM1_VECTORn, 2);                         //���벶׽�ж�������һ·�ٶ�
                NVIC_SetPriority(UART4_RX_TX_VECTORn, 0); 
                gpio_init (PTD15,GPO, 0);
  /*������������������������������ʼ����������������������������*/
                gpio_init (PTA16,GPO, 0);
                gpio_init (PTE26,GPO, 1);
  /*����������������������������ʼ������������������������*/
                gpio_init (KEY_A,GPI,0);
                gpio_init (KEY_B,GPI,0);
                gpio_init (KEY_C,GPI,0);
                gpio_init (KEY_D,GPI,0);
                gpio_init (KEY_E,GPI,0);
/*�������������������������뿪�س�ʼ������������������������*/
                gpio_init (SWITCH_A,GPI,0);
                gpio_init (SWITCH_B,GPI,0);
                gpio_init (SWITCH_C,GPI,0);
                gpio_init (SWITCH_D,GPI,0);      

  /*������������������������LED��ʾ����ʼ������������������������������*/
                        LED_Init();
  /*�������������������������������ٳ�ʼ����������������������������*/
                ftm_input_init(FTM1,FTM_CH0,FTM_Falling,FTM_PS_1);            
                port_init_NoALT(Left_Pulse_Pin ,PULLUP);                       
                set_vector_handler(FTM1_VECTORn ,FTM1_INPUT_IRQHandler);
                enable_irq(FTM1_IRQn);
                gpio_init(Left_Direction_Pin,GPI,0);                                      
  /*���������������������ҵ�����ٳ�ʼ��������������������������*/
                lptmr_pulse_init(LPT0_ALT2,0xFFFF,LPT_Rising);              
                gpio_init(Right_Direction_Pin,GPI,0);                        
  /*�����������������������������ʼ������������������������������*/
                ftm_pwm_init(FTM0, FTM_CH0,8000, 0);//�ҷ� 
                ftm_pwm_init(FTM0, FTM_CH1,8000, 0);//��ǰ
                ftm_pwm_init(FTM0, FTM_CH2,8000, 0);//��ǰ
                ftm_pwm_init(FTM0, FTM_CH3,8000, 0);//�� 
  /*��������������������������������ʼ��������������������������*/
                adc_init(AD_CH0);
                adc_init(AD_CH1);
                adc_init(AD_CH2);
                adc_init(AD_CH3);
                ftm_pwm_init(FTM2, FTM_CH1,100, 1400);                        
  /*������������������������uart��ʼ������������������������������*/
                uart_init(UART4,115200);
                set_vector_handler(UART4_RX_TX_VECTORn,uart4_test_handler);   
                uart_rx_irq_en(UART4);
                uart_init(UART0,9600);
                set_vector_handler(UART0_RX_TX_VECTORn,uart0_test_handler);   
                uart_rx_irq_en(UART0);
               // uart_init(UART1,9600);
  /*��������������������PIT��ʱ����ʼ����CCD�����١������ã�����������������*/
                pit_init_ms(PIT0,8);                                  //��ʼ��PIT0ģ�飬����1ms��һ���ж�
                set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);     //���жϷ������ӵ��ж���������
                enable_irq(PIT0_IRQn);                                //����PIT0�ж�
  /*��������������������I2C�����Լ�MPU9250�ĳ�ʼ������������������������������*/
                IIC_Init();//ģ��I2C�����ĳ�ʼ����һ��Ҫ�ȳ�ʼ��I2C�ӿڲ��ܲ���MPU9250оƬ
                Init_MPU9250_With_DMP(); 

}




extern float Distance_ht,Distance_hr,Distance_tr,Angle_of_Target; 
extern float Yaw,Yaw_Ref; 
extern int16 Rear_x ,Rear_y ,Token_x , Token_y, Head_x, Head_y;
extern uint8 Key_Val_1,Key_Val_2,Key_Val_3,Key_Val_4,LED_Show_Flag;   //�洢��ֵ�ı���(��־λ)
extern float AngleError; 
extern struct SAngle stcAngle;
extern int16 Left_Speed,Right_Speed,Sys_Count;
extern int16 L_Set_Speed,R_Set_Speed;
void main(void)   
{
    init();         //��ʼ������
    while(1)
    {

        MPU9250_search();
        Key_Scan();
        if(Key_Val_1 == 1)           //��ʾ��Ҫ��ƫ����Ϣ
        {
          LED_P6x8Str(0,0,"Yaw"); 
          LED_Show_Flag = 1;
          LED_PrintValueI(60,1,(int)Yaw);      
          LED_P6x8Str(0,2,"Angle_TC");   
          LED_PrintValueI(60,3,(int)Angle_of_Target);
          LED_P6x8Str(0,4,"AngleError:");          
          LED_PrintValueI(60,5,(int)AngleError);
        }
        else if(Key_Val_2 == 1)   //��ʾ �й�AD����ֵ����Ϣ
        {
          LED_Show_Flag = 1;                    //8.396*10^(-5)*adc_once(BIG_EYE0, ADC_10bit)^2 -0.1758*adc_once(BIG_EYE0, ADC_10bit) + 110.3   AD����ת����ʽ
          LED_PrintValueI(60,2,AD_To_Distance(AD_CH1, ADC_10bit));   
          LED_PrintValueI(60,4,AD_To_Distance(AD_CH2, ADC_10bit));  
          LED_PrintValueI(60,6,AD_To_Distance(AD_CH3, ADC_10bit)); 

          
        }
        else if(Key_Val_3 == 1)  //��ʾ�ɼ����Ĺؼ���������Ϣ
        {
          LED_Show_Flag = 1;
        
          LED_P6x8Str(0,1,"Rear:");
          LED_PrintValueI(0,2,(int)Rear_x);   LED_PrintValueI(50,2,(int)Rear_y);
          LED_P6x8Str(0,3,"Token:");
          LED_PrintValueI(0,4,(int)Token_x);   LED_PrintValueI(50,4,(int)Token_y);
          LED_P6x8Str(0,5,"Head:");
          LED_PrintValueI(0,6,(int)Head_x);   LED_PrintValueI(50,6,(int)Head_y);
        }
        else if(Key_Val_4 == 1)   //��ʾ�ⶨ�������Ϣ
        {
          LED_Show_Flag = 1;
          LED_P6x8Str(0,0,"Distance_tr");
          LED_P6x8Str(0,2,"Distance_hr");
          LED_P6x8Str(0,4,"Distance_ht");
          LED_PrintValueI(60,1,(int)Distance_tr);
          LED_PrintValueI(60,3,(int)Distance_hr);       //����ĳ����Ǽ�������� ������ʵ�ĳ���
          LED_PrintValueI(60,5,(int)Distance_ht);
          LED_P6x8Str(0,6,"Distance ^2:");
          LED_PrintValueI(60,7,(Rear_x-Token_x)^2 + (Rear_y - Token_y)^2);
        }
        else if( LED_Show_Flag == 1)
        {
          LED_CLS();     //��������
          LED_Show_Flag = 0;
        }
     // Stevos_Test();           //������Ժ���,����ʱ������������
    }
}
