#ifndef __SERIAL_GYRO_H
#define __SERIAL_GYRO_H		
struct SAngle
{
	short Angle[3];
	short T;
};

extern struct SAngle 		stcAngle;
void CopeSerialData(unsigned char ucData);
#endif