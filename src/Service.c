#include "include.h"
#include "math.h"


/*!
  ��������double Distance_Measure(int p1_x,int p1_y,int p2_x ,int p2_y)
  �������ܣ����������
  ʵ�Σ�������ĺ�������ֵ
  ����ֵ��������֮��ľ���
*/

double Distance_Measure(int p1_x,int p1_y,int p2_x ,int p2_y)
{
    double c;
    c=sqrt((p1_x-p2_x)*(p1_x-p2_x)+(p1_y-p2_y)*(p1_y-p2_y));
    return c;
}


/*!
  ��������double angle_cos(double a,double b,double c);
  �������ܣ����Ҷ�����Ƕ�
  ʵ�Σ����Ҷ���ȷ���������� a��b��c
  ����ֵ���Ƕ�
*/
double Angle_Measure(double a,double b,double c)
{
    double cos_a;
    cos_a=(b*b+c*c-a*a)/(2*b*c);
    cos_a=acos(cos_a)/3.14*180;
    return cos_a;     //ֱ�ӽ�����Ƕ�ֵ
}

/*��������������������������������������������������������������������
 *  @brief       Key_Scan����
 *  @since       v1.0
 *  @note       �������º󣬸ð����ļ����־λ��1���ٴΰ��±�־λ��0
              ��������������
 ��������������������������������������������������������������������*/
uint8 Key_Val_1=0,Key_Val_2=0,Key_Val_3=0,Key_Val_4=0,LED_Show_Flag=0;   //�洢��ֵ�ı���(��־λ)
void Key_Scan(void)
{
  if(gpio_get(KEY_A) == 0)
  {
    pit_delay_ms(PIT3,100);
    if(gpio_get(KEY_A) == 0)
      Key_Val_1 = 1 - Key_Val_1;
  }
  else if(gpio_get(KEY_B) == 0)
  {
    pit_delay_ms(PIT3,100);
    if(gpio_get(KEY_B) == 0)
      Key_Val_2 = 1 - Key_Val_2;
  }
  else if(gpio_get(KEY_C) == 0)
  {
    pit_delay_ms(PIT3,100);
    if(gpio_get(KEY_C) == 0)
      Key_Val_3 = 1 - Key_Val_3;
  }
  else if(gpio_get(KEY_D) == 0)
  {
    pit_delay_ms(PIT3,100);
    if(gpio_get(KEY_D) == 0)
      Key_Val_4 = 1 - Key_Val_4;
  }
}

/*��������������������������������������������������������������������������������
*  @brief      Stevos_Test����
*  @since      v1.0
*  @note       ���ƫ�ǵ��ں��� (���ڲ��������ǵ���ֵ����ֵ)��ʹ��ʱ����ѭ��
            �е�������
��������������������������������������������������������������������������������*/
uint32 Stevos_out = 1520,modify_num = 10;
void Stevos_Test(void)
{
  if(gpio_get(KEY_A) == 0)
  {
    while(gpio_get(KEY_A) == 0);
      Stevos_out += modify_num ;//�����ƫ�� ÿ�ε��� modify_num 
  }
  if(gpio_get(KEY_C) == 0)
  {
    while(gpio_get(KEY_C) == 0);
      Stevos_out -= modify_num ;//�����ƫ�� ÿ�εݼ� modify_num 
  }
  if(gpio_get(KEY_E) == 0)
  {
    while(gpio_get(KEY_E) == 0);
      modify_num = 11 - modify_num;  //�л����ƫ��ÿ�εĵݱ�ֵ �� 1 �� 10 ֮���໥�л�
  }
  LED_P6x8Str(0,0,"Stevos_out:");
  LED_PrintValueI(90,0,Stevos_out);
  
  LED_P6x8Str(0,1,"modify_num:");
  LED_PrintValueI(90,1,modify_num);   //��ʾ����
  
  ftm_pwm_duty(Stevos_FTM, Stevos_CH, Stevos_out);// ���ƶ��
  
}

int AD_To_Distance(ADCn_Ch_e adcn_ch, ADC_nbit bit)
{
  if(adc_once(adcn_ch, bit)<270)
    return 109;
  else
    return  8.396*0.00001*adc_once(adcn_ch, bit)*adc_once(adcn_ch, bit) -0.1758*adc_once(adcn_ch, bit) + 110.3;
}




