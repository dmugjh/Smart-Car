#ifndef __SERVICE_H__
#define __SERVICE_H__

void Stevos_Test(void);
void Key_Scan(void);
double Distance_Measure(int p1_x,int p1_y,int p2_x ,int p2_y);
double Angle_Measure(double a,double b,double c);
int AD_To_Distance(ADCn_Ch_e adcn_ch, ADC_nbit bit);

#endif