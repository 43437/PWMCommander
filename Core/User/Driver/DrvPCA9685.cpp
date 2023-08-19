#include "DrvPCA9685.h"
#include "../Dev/DevPCA9685.h"

extern I2C_HandleTypeDef hi2c1;

DrvPCA9685& DrvPCA9685::Instance(){
	static DrvPCA9685 __instance;
	return __instance;
}

DrvPCA9685::DrvPCA9685(){
	Init();
}

void DrvPCA9685::Init(){
	DevPCA9685::Instance().SetI2CHandle(&hi2c1);
	DevPCA9685::Instance().Init();
}
