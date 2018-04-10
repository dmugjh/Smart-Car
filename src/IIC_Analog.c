/* IOI2C.c file
�ṩI2C�ӿڲ���API ��
ʹ��IOģ�ⷽʽ
#define IIC_SCL    PD6 //SCL
#define IIC_SDA    PD7 //SDA
-------------------------- */
#include "IIC_Analog.h"
#include  "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_SysTick.h"
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Init(void)
*��������:		��ʼ��I2C��Ӧ�Ľӿ����š�
*******************************************************************************/
void IIC_Init(void) //9250�ӿ�
{
   gpio_init(PTD6,GPO,1);
   gpio_init(PTD7,GPO,1);
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Start(void)
*��������:		����IIC��ʼ�ź�
*******************************************************************************/
void IIC_Start(void)
{
	IIC_SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	systick_delay(delay_us_4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	systick_delay(delay_us_4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Stop(void)
*��������:	    //����IICֹͣ�ź�
*******************************************************************************/	  
void IIC_Stop(void)
{
	IIC_SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	systick_delay(delay_us_4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	systick_delay(delay_us_4);							   	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8 IIC_Wait_Ack(void)
*��������:	    �ȴ�Ӧ���źŵ��� 
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
*******************************************************************************/
uint8 IIC_Wait_Ack(void)
{
	uint8 ucErrTime=0;
        // OLED_Display_5_int(3,3,100);
	IIC_SDA_IN();      //SDA����Ϊ����  
      //  OLED_Display_5_int(3,3,100);
	IIC_SDA=1;
        
        systick_delay(delay_us_1);
        
	IIC_SCL=1;
        systick_delay(delay_us_1);	
        
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 1;
		}
	  systick_delay(delay_us_1);
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Ack(void)
*��������:	    ����ACKӦ��
*******************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT();
	IIC_SDA=0;
	systick_delay(delay_us_2);
	IIC_SCL=1;
	systick_delay(delay_us_2);
	IIC_SCL=0;
}
	
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_NAck(void)
*��������:	    ����NACKӦ��
*******************************************************************************/	    
void IIC_NAck(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT();
	IIC_SDA=1;
	systick_delay(delay_us_2);
	IIC_SCL=1;
	systick_delay(delay_us_2);
	IIC_SCL=0;
}					 				     

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Send_Byte(uint8 txd)
*��������:	    IIC����һ���ֽ�
*******************************************************************************/		  
void IIC_Send_Byte(uint8 txd)
{                        
    uint8 t;   
	IIC_SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
      //  IIC_SDA=(txd&0x80)>>7;
        gpio_set(PTD7,(txd&0x80)>>7); 
        txd<<=1; 	  
		systick_delay(delay_us_2);   
		IIC_SCL=1;
		systick_delay(delay_us_2); 
		IIC_SCL=0;	
		systick_delay(delay_us_2);
    }	 
} 	 
   
/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8 IIC_Read_Byte(unsigned char ack)
*��������:	    //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK 
*******************************************************************************/  
uint8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        systick_delay(delay_us_2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		systick_delay(delay_us_2); 
    }					 
    if (ack)
        IIC_Ack(); //����ACK 
    else
        IIC_NAck();//����nACK  
    return receive;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	I2C_Addr  Ŀ���豸��ַ
		addr	   �Ĵ�����ַ
����   ��������ֵ
*******************************************************************************/ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	IIC_Start();	
	IIC_Send_Byte(I2C_Addr);	   //����д����
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr); res++;  //���͵�ַ
	IIC_Wait_Ack();	  
	//IIC_Stop();//����һ��ֹͣ����	
	IIC_Start();
	IIC_Send_Byte(I2C_Addr+1); res++;          //�������ģʽ			   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);	   
    IIC_Stop();//����һ��ֹͣ����

	return res;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8 IICreadBytes(uint8 dev, uint8 reg, uint8 length, uint8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫ�����ֽ���
		*data  ���������ݽ�Ҫ��ŵ�ָ��
����   ���������ֽ�����
*******************************************************************************/ 
uint8 IICreadBytes(uint8 dev, uint8 reg, uint8 length, uint8 *data){
    uint8 count = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //���͵�ַ
    IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte(dev+1);  //�������ģʽ	
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  //��ACK�Ķ�����
		 	else  data[count]=IIC_Read_Byte(0);	 //���һ���ֽ�NACK
	}
    IIC_Stop();//����һ��ֹͣ����
    return count;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8 IICwriteBytes(uint8 dev, uint8 reg, uint8 length, uint8* data)
*��������:	    ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫд���ֽ���
		*data  ��Ҫд�����ݵ��׵�ַ
����   �����Ƿ�ɹ�
*******************************************************************************/ 
uint8 IICwriteBytes(uint8 dev, uint8 reg, uint8 length, uint8* data){
  
 	uint8 count = 0;
	IIC_Start();
        
	IIC_Send_Byte(dev);	   //����д����
        
	IIC_Wait_Ack();
    //    OLED_Display_5_int(3,3,100);
	IIC_Send_Byte(reg);   //���͵�ַ
        IIC_Wait_Ack();	  
      
	for(count=0;count<length;count++){
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
	 }
	IIC_Stop();//����һ��ֹͣ����

    return 1; //status == 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8 IICreadByte(uint8 dev, uint8 reg, uint8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		*data  ���������ݽ�Ҫ��ŵĵ�ַ
����   1
*******************************************************************************/ 
uint8 IICreadByte(uint8 dev, uint8 reg, uint8 *data){
	*data=I2C_ReadOneByte(dev, reg);
    return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*��������:	    д��ָ���豸 ָ���Ĵ���һ���ֽ�
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		data  ��Ҫд����ֽ�
����   1
*******************************************************************************/ 
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data){
    return IICwriteBytes(dev, reg, 1, &data);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8 IICwriteBits(uint8 dev,uint8 reg,uint8 bitStart,uint8 length,uint8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitStart  Ŀ���ֽڵ���ʼλ
		length   λ����
		data    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
uint8 IICwriteBits(uint8 dev,uint8 reg,uint8 bitStart,uint8 length,uint8 data)
{

    uint8 b;
    if (IICreadByte(dev, reg, &b) != 0) {
        uint8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IICwriteByte(dev, reg, b);
    } else {
        return 0;
    }
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8 IICwriteBit(uint8 dev, uint8 reg, uint8 bitNum, uint8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
		data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
uint8 IICwriteBit(uint8 dev, uint8 reg, uint8 bitNum, uint8 data){
    uint8 b;
    IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------
