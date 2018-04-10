#include "include.h"
#include "math.h"


/*!
  函数名：double Distance_Measure(int p1_x,int p1_y,int p2_x ,int p2_y)
  函数功能：两点求距离
  实参：两个点的横纵坐标值
  返回值：两个点之间的距离
*/

double Distance_Measure(int p1_x,int p1_y,int p2_x ,int p2_y)
{
    double c;
    c=sqrt((p1_x-p2_x)*(p1_x-p2_x)+(p1_y-p2_y)*(p1_y-p2_y));
    return c;
}


/*!
  函数名：double angle_cos(double a,double b,double c);
  函数功能：余弦定理求角度
  实参：余弦定理确定的三条边 a、b、c
  返回值：角度
*/
double Angle_Measure(double a,double b,double c)
{
    double cos_a;
    cos_a=(b*b+c*c-a*a)/(2*b*c);
    cos_a=acos(cos_a)/3.14*180;
    return cos_a;     //直接解算出角度值
}

/*——————————————————————————————————
 *  @brief       Key_Scan函数
 *  @since       v1.0
 *  @note       按键按下后，该按键的键码标志位置1，再次按下标志位置0
              服务于主函数中
 ——————————————————————————————————*/
uint8 Key_Val_1=0,Key_Val_2=0,Key_Val_3=0,Key_Val_4=0,LED_Show_Flag=0;   //存储键值的变量(标志位)
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

/*————————————————————————————————————————
*  @brief      Stevos_Test函数
*  @since      v1.0
*  @note       舵机偏角调节函数 (用于测量舵机打角的阈值、中值)，使用时放在循环
            中单独调用
————————————————————————————————————————*/
uint32 Stevos_out = 1520,modify_num = 10;
void Stevos_Test(void)
{
  if(gpio_get(KEY_A) == 0)
  {
    while(gpio_get(KEY_A) == 0);
      Stevos_out += modify_num ;//舵机的偏量 每次递增 modify_num 
  }
  if(gpio_get(KEY_C) == 0)
  {
    while(gpio_get(KEY_C) == 0);
      Stevos_out -= modify_num ;//舵机的偏量 每次递减 modify_num 
  }
  if(gpio_get(KEY_E) == 0)
  {
    while(gpio_get(KEY_E) == 0);
      modify_num = 11 - modify_num;  //切换舵机偏量每次的递变值 在 1 和 10 之间相互切换
  }
  LED_P6x8Str(0,0,"Stevos_out:");
  LED_PrintValueI(90,0,Stevos_out);
  
  LED_P6x8Str(0,1,"modify_num:");
  LED_PrintValueI(90,1,modify_num);   //显示函数
  
  ftm_pwm_duty(Stevos_FTM, Stevos_CH, Stevos_out);// 控制舵机
  
}

int AD_To_Distance(ADCn_Ch_e adcn_ch, ADC_nbit bit)
{
  if(adc_once(adcn_ch, bit)<270)
    return 109;
  else
    return  8.396*0.00001*adc_once(adcn_ch, bit)*adc_once(adcn_ch, bit) -0.1758*adc_once(adcn_ch, bit) + 110.3;
}




