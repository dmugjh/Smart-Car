#include "include.h"

struct SAngle stcAngle={0};
char Uart0_Recv_Flag = 0;
void CharToLong(char Dest[],char Source[])
{
	 *Dest 		= Source[3];
	 *(Dest+1) 	= Source[2];
	 *(Dest+2) 	= Source[1];
	 *(Dest+3) 	= Source[0];
}

void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[12];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;																																	  
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		if(ucRxBuffer[1] == 0x53)
		{
                  stcAngle.Angle[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[2];
		  stcAngle.Angle[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[4];
		  stcAngle.Angle[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[6];
//                  stcAngle.Angle[2] = stcAngle.Angle[2]*1.0/32768*180;    //��Ӧ�ľ���ƫ����
                  Uart0_Recv_Flag = 1;
		}
		ucRxCnt=0;
	}
}