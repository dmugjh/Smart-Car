#ifndef _MPU6050_H
#define _MPU6050_H

#include "dmp.h"
#include "IIC_Analog.h"
#include "math.h"
#include "MK60_SysTick.h"
#include "dmp_api.h"
#include "MK60_gpio.h"

void MPU9250_INT_init(void);
void MPU9250_search(void);

#endif