#include "include.h"
#include "control.h"

#define Image_H  240
#define Image_L  320

/*!
 *  @brief      ���������ƺ���
 *  @date       2017/6/11
 *  @note       
 */
uint8 Turn_array[14] = {0 ,0 ,1 ,1 ,1 ,1 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,0};   
int8 Obstacle_Flag = 0;    //�� 0 ��ʾ�����ϰ��� 1 ��ʾ��⵽�ϰ���
int8 Stop_Flag = 0 ,Recov_Flag1 = 0,Recov_Flag2 = 0;
int16 Now_Speed,Set_Speed;
float Yaw,Yaw_Ref; //ƫ����
float AnglePID ,StevosAngle ,Prev_AngleError = 0,AngleError = 0;
float Distance_ht = 0, Distance_hr = 0, Distance_tr = 0, Angle_of_Target = 0;     //�����Լ����ű�����ɵ�ʸ���ļн�
extern struct SAngle stcAngle;
extern int16 Rear_x ,Rear_y ,Token_x , Token_y ;
int16 Token_x_Prev = 0,Token_y_Prev = 0;
/*���������������������������������������������������������������������������������ƺ�������������������������������������������������������������������������*/
void Servo_Control(void)
{
    static float Angle_P = 5.2,Angle_D = 3.2;   //ԭ���� 5.2 1.2
  //  LED_PrintValueI(60,0,icount);
    Distance_tr = Distance_Measure(Rear_x,Rear_y,Token_x ,Token_y);  //��¼��ͷ���ű�Ƶľ���
    if(Rear_y - Distance_tr <0)
    {
      Distance_hr = Rear_y;
      Distance_ht = Distance_Measure(Rear_x ,0,Token_x,Token_y);
    }
    else
    {
      Distance_hr = Distance_tr;
      Distance_ht = Distance_Measure(Rear_x ,(int)(Rear_y- Distance_tr),Token_x,Token_y);
    }
    
      if( Rear_x < Token_x)     //a �ǳ�ͷ���ű�  b���ű�ͳ�β  c�ǳ�ͷ�ͳ�β ��һ���� b == c
        Angle_of_Target = Angle_Measure(Distance_ht,Distance_tr,Distance_hr);
      else 
        Angle_of_Target = -Angle_Measure(Distance_ht,Distance_tr,Distance_hr);
      
     if( abs(Token_x - Rear_x)<10 && Rear_y <Token_y)   Angle_of_Target = 179;

    /*�������������������������ٵ� �ؾ� 58.0������������������*/
    if(Distance_tr<110 && Now_Speed >160)
        Angle_of_Target = Angle_of_Target +(56.0 - 0.8665*Distance_tr+0.004*Distance_tr*Distance_tr); 
    else if(Distance_tr <110 &&(Token_y <60||Token_y>180))      //�е��ٵ� ƫ���ڶ���͵Ͳ� �ؾ���С
        Angle_of_Target = Angle_of_Target +(53.0 - 0.8665*Distance_tr+0.004*Distance_tr*Distance_tr);
    else if(Distance_tr<110)        //�������ؾ���С
        Angle_of_Target = Angle_of_Target +(51.0 - 0.8665*Distance_tr+0.004*Distance_tr*Distance_tr);

    if( Distance_tr<80 && Distance_tr>0 )         
    {
        Angle_P = 4.2,Angle_D = 3.2;  // 4.2 2    
    }
    else
    {
        Angle_P = 2.5,Angle_D = 1.5;
    }
    
    AngleError = Yaw - Angle_of_Target ;
    
    if(AngleError >180) AngleError = AngleError- 360;
    if(AngleError <-180 ) AngleError = AngleError+360;
    
    
  AnglePID = AngleError*Angle_P+Angle_D*(AngleError-Prev_AngleError);
  
  StevosAngle = (int16)(Mid_Angle+AnglePID); 
  Prev_AngleError = AngleError;

  /*���������������������������ϴ�������������������������*/
  if(gpio_get(SWITCH_A) == 0)
  {
    if(fabs(AngleError) <30)
    {
      if(AD_To_Distance(AD_CH2, ADC_10bit)<80)
      {
        StevosAngle = 1670;//1670
        Obstacle_Flag = 1;
      }
      else if(AD_To_Distance(AD_CH1, ADC_10bit)<80)
      {
        StevosAngle = 1640;//1640
        Obstacle_Flag = 1;
      }
      else if(AD_To_Distance(AD_CH3, ADC_10bit)<80)
      {
        StevosAngle = 1390;//1390
        Obstacle_Flag = 1;
      }
      else
        Obstacle_Flag = 0;
    }
    else
      Obstacle_Flag = 0;
  }


  if( Stop_Flag == 1)
  {
      StevosAngle=Min_Angle ; //�ܳ����غ���ƫ�ǹ̶�
  }
  
  
  /*�������������������Ʒ��ȣ���ֹ�ջ��������������������*/
                if(StevosAngle>=Max_Angle)
                  StevosAngle = Max_Angle;
                if(StevosAngle<=Min_Angle)
                  StevosAngle = Min_Angle; 
	
  ftm_pwm_duty(Stevos_FTM, Stevos_CH, (int16)StevosAngle);// ���ƶ��
  

}

/*!
 *  @brief      ���������ƺ���
 *  @date       2017/6/11
 *  @note       
 */


extern int16 Left_Speed,Right_Speed;  
float Motor_P=6.0,Motor_I=0.6,Motor_D=0.6;      // �ٶȿ���PID   6.0        0.6          0.6
int16    L_Speed_LR_Ek0=0,                              //   8.0     0.6    1.2
         L_Speed_LR_Ek1=0,
         L_Speed_LR_Ek2=0;              //�����ٶȵĸ���ƫ����	
int16    R_Speed_LR_Ek0=0,
         R_Speed_LR_Ek1=0,
         R_Speed_LR_Ek2=0;              //�����ٶȵĸ���ƫ����
int16 L_Set_Speed,R_Set_Speed;	        //�������趨���ٶ�
int16 LeftMotor_Duty=0,RightMotor_Duty=0;       //������������ռ�ձ�
extern int16 Sys_Count;

void Speed_Control(void)
{
    int16 Motor_Duty_Ch =0; 
    Set_Speed = 110;
    if(Now_Speed >160) Set_Speed = 70;
    if(Now_Speed >180) Set_Speed = 50;
    Motor_Duty_Ch = (int16)((StevosAngle - Mid_Angle)/1.5 );  //17.1s ��ʱ����3
    if(gpio_get(SWITCH_B) == 0 &&Distance_tr>60 &&Distance_tr <= 100&& fabs(AngleError) <90)  
    {
      Set_Speed = 150;  //���Խ׶� 11 0 ���� 160
      Motor_Duty_Ch = (int16)(StevosAngle - Mid_Angle)/6;
    }
    else if(gpio_get(SWITCH_C) == 0 &&Distance_tr>100 && fabs(AngleError) <90)
    {
      Set_Speed = 230;
      Motor_Duty_Ch = (int16)(StevosAngle - Mid_Angle)/6;
   //   gpio_set(PTA16,1);
    }
    if(Obstacle_Flag == 1)  Motor_Duty_Ch = (int16)(StevosAngle - Mid_Angle)/3;
    

    Now_Speed = (Left_Speed+Right_Speed)/2;         //���ݱ�������������ֵ�õ���ǰ���ٶ�
    
    L_Set_Speed=Set_Speed-Motor_Duty_Ch;            //�������趨���ٶ�
    R_Set_Speed=Set_Speed+Motor_Duty_Ch;
    
    if(Recov_Flag1 == 1 &&Recov_Flag2 == 1 && Sys_Count <100) Stop_Flag = 0;

    if( Stop_Flag == 1)
    {
      L_Set_Speed = 0 , R_Set_Speed =0;       //�ܳ����غ�ͣ��
    }
    /*��������������������������PID���Ƶ�����֡���������������������������������*/
    //����
    L_Speed_LR_Ek0 = L_Set_Speed - Left_Speed;
        
    LeftMotor_Duty  = LeftMotor_Duty 
                                     + (int16)(Motor_P*(L_Speed_LR_Ek0-L_Speed_LR_Ek1))
                                     + (int16)(Motor_I*L_Speed_LR_Ek0)
                                     + (int16)(Motor_D*(L_Speed_LR_Ek0-2*L_Speed_LR_Ek1+L_Speed_LR_Ek2));

    L_Speed_LR_Ek2 = L_Speed_LR_Ek1;
    L_Speed_LR_Ek1 = L_Speed_LR_Ek0;                  //���´洢�Ĳ�ֵ
        
        //����
    R_Speed_LR_Ek0 = R_Set_Speed - Right_Speed;
        
    RightMotor_Duty  =RightMotor_Duty 
                                      + (int16)(Motor_P*(R_Speed_LR_Ek0-R_Speed_LR_Ek1))
                                      + (int16)(Motor_I*R_Speed_LR_Ek0)
                                      + (int16)(Motor_D*(R_Speed_LR_Ek0-2*R_Speed_LR_Ek1+R_Speed_LR_Ek2));
	
    R_Speed_LR_Ek2 = R_Speed_LR_Ek1;
    R_Speed_LR_Ek1 = R_Speed_LR_Ek0;                //���´洢�Ĳ�ֵ
	
        
    if(LeftMotor_Duty >950)    LeftMotor_Duty =950;    //���Ƶ����ռ�ձȣ����90%ռ�ձ�
    if(LeftMotor_Duty <=-300)  LeftMotor_Duty =-300;
    if(RightMotor_Duty >950)   RightMotor_Duty =950;
    if(RightMotor_Duty <=-300) RightMotor_Duty =-300;
    
    
  
      if(LeftMotor_Duty>=0)//��ת
      {
        ftm_pwm_duty(FTM0,FTM_CH0,0);
        ftm_pwm_duty(FTM0,FTM_CH1,LeftMotor_Duty);
      }
      else//��ת����
      {
        ftm_pwm_duty(FTM0,FTM_CH0,0-LeftMotor_Duty);
        ftm_pwm_duty(FTM0,FTM_CH1,0);
      } 
      //����
      if(RightMotor_Duty>=0)//��ת
      {
        ftm_pwm_duty(FTM0, FTM_CH2,RightMotor_Duty);//��ǰ
        ftm_pwm_duty(FTM0, FTM_CH3,0);//�ҷ�
      }
      else//��ת����
      {
        ftm_pwm_duty(FTM0, FTM_CH2,0);//��ǰ
        ftm_pwm_duty(FTM0, FTM_CH3,0-RightMotor_Duty);//�ҷ�
      }
 

}
int duty1 = 200;
void Speed_Control_Test(void)
{
  

    ftm_pwm_duty(FTM0, FTM_CH0,0);
    ftm_pwm_duty(FTM0, FTM_CH1,200);
    ftm_pwm_duty(FTM0, FTM_CH2,0);
    ftm_pwm_duty(FTM0, FTM_CH3,200);
}

void Servo_Control_test(void)
{
  float Angle_P = 5.2,Angle_D = 1.2; 
    static float Angle_of_Target_temp = 90 ,modi_num = 1;
    if(gpio_get(KEY_A) == 0)
    {
      pit_delay_ms(PIT3,100);
      if(gpio_get(KEY_A) == 0)
        Angle_of_Target_temp = Angle_of_Target_temp +modi_num;
    }
    else if(gpio_get(KEY_B) == 0)
    {
      pit_delay_ms(PIT3,100);
      if(gpio_get(KEY_B) == 0)
        Angle_of_Target_temp = Angle_of_Target_temp -modi_num;
    }
    else if(gpio_get(KEY_C) == 0)
    {
      pit_delay_ms(PIT3,100);
      if(gpio_get(KEY_C) == 0)
        modi_num = 6 - modi_num;
    }
    Distance_tr = Distance_Measure(Rear_x,Rear_y,Token_x ,Token_y);  //��¼��ͷ���ű�Ƶľ���
    if(Rear_y - Distance_tr <0)
    {
      Distance_hr = Rear_y;
      Distance_ht = Distance_Measure(Rear_x ,0,Token_x,Token_y);
    }
    else
    {
      Distance_hr = Distance_tr;
      Distance_ht = Distance_Measure(Rear_x ,(int)(Rear_y- Distance_tr),Token_x,Token_y);
    }
    
      if( Rear_x < Token_x)
        Angle_of_Target = Angle_Measure(Distance_ht,Distance_tr,Distance_hr);
      else
        Angle_of_Target = -Angle_Measure(Distance_ht,Distance_tr,Distance_hr);
    
    
    AngleError = Yaw - Angle_of_Target_temp ;
   
    if(AngleError >180) AngleError = AngleError- 360;
    if(AngleError <-180 ) AngleError = AngleError+360;
    
  /*���������������������������PID����������������������*/

  AnglePID = AngleError*Angle_P
                        +Angle_D*(AngleError-Prev_AngleError);
  StevosAngle = (int16)(Mid_Angle+AnglePID); 
  Prev_AngleError = AngleError;

  
  /*�������������������Ʒ��ȣ���ֹ�ջ��������������������*/
                if(StevosAngle>=Max_Angle)
                  StevosAngle = Max_Angle;
                if(StevosAngle<=Min_Angle)
                  StevosAngle = Min_Angle; 
	
  ftm_pwm_duty(Stevos_FTM, Stevos_CH, (int16)StevosAngle);// ���ƶ��

}


