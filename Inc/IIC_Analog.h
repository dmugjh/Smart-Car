#ifndef __IOI2C_H
#define __IOI2C_H
#include "common.h"
#define delay_us_1 180
#define delay_us_2 360
#define delay_us_4 720
//IO��������
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}//PB7 ����/��������ģʽ
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}//PB7 ͨ��������� 50Mhz
//IO��������
//#define IIC_SCL    PBout(6) //SCL
//#define IIC_SDA    PBout(7) //SDA	 
//#define READ_SDA   PBin(7)  //����SDA 

//IO��������
#define IIC_SDA_IN()  gpio_ddr(PTD7,GPI)//PB7 ����/��������ģʽ
#define IIC_SDA_OUT() gpio_ddr(PTD7,GPO)//PB7 ͨ��������� 50Mhz

//IO��������	 
#define IIC_SCL    PTD6_OUT//SCL
#define IIC_SDA    PTD7_OUT //SDA
 //SDA //����	 
#define READ_SDA     gpio_get(PTD7)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8 txd);			//IIC����һ���ֽ�
uint8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

//void IIC_Write_One_Byte(uint8 daddr,uint8 addr,uint8 data);
//uint8 IIC_Read_One_Byte(uint8 daddr,uint8 addr);	 
//unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
uint8 IICwriteBytes(uint8 dev, uint8 reg, uint8 length, uint8* data);
uint8 IICwriteBits(uint8 dev,uint8 reg,uint8 bitStart,uint8 length,uint8 data);
uint8 IICwriteBit(uint8 dev,uint8 reg,uint8 bitNum,uint8 data);
uint8 IICreadBytes(uint8 dev, uint8 reg, uint8 length, uint8 *data);

#endif

//------------------End of File----------------------------

