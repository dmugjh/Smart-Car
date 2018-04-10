#ifndef __IOI2C_H
#define __IOI2C_H
#include "common.h"
#define delay_us_1 180
#define delay_us_2 360
#define delay_us_4 720
//IO方向设置
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}//PB7 上拉/下拉输入模式
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}//PB7 通用推挽输出 50Mhz
//IO操作函数
//#define IIC_SCL    PBout(6) //SCL
//#define IIC_SDA    PBout(7) //SDA	 
//#define READ_SDA   PBin(7)  //输入SDA 

//IO方向设置
#define IIC_SDA_IN()  gpio_ddr(PTD7,GPI)//PB7 上拉/下拉输入模式
#define IIC_SDA_OUT() gpio_ddr(PTD7,GPO)//PB7 通用推挽输出 50Mhz

//IO操作函数	 
#define IIC_SCL    PTD6_OUT//SCL
#define IIC_SDA    PTD7_OUT //SDA
 //SDA //卡死	 
#define READ_SDA     gpio_get(PTD7)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8 txd);			//IIC发送一个字节
uint8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

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

