#include "DevPCA9685.h"
#include <math.h>

#define NULLCHECK 					\
	do{								\
		if (nullptr == m_hi2c) { 	\
			return;					\
		}							\
	}while(0);

#define NULLCHECK_RET_U8			\
	do{								\
		if (nullptr == m_hi2c) { 	\
			return 0;				\
		}							\
	}while(0);

DevPCA9685& DevPCA9685::Instance(){
	static DevPCA9685 __instance;
	return __instance;
}

void DevPCA9685::SetPWM(const EPort ePort, uint16_t on, uint16_t off) {

	Write(LED0_ON_L+4*ePort,on);
	Write(LED0_ON_H+4*ePort,on>>8);
	Write(LED0_OFF_L+4*ePort,off);
	Write(LED0_OFF_H+4*ePort,off>>8);
}

void DevPCA9685::SetPWMPercent(const EPort ePort, uint8_t percent){

	uint16_t highCNT = (uint64_t)CNT * (uint64_t)percent / 100;
	SetPWM(ePort, 0, highCNT);
}

void DevPCA9685::SetPWMDegree(const EPort ePort, uint8_t degree){

	if (degree > 180)
		degree = 180;

	if (degree <= 0)
		degree = 0;

	auto v_us = degree * 1800 / 180 + 600;	//0.6~2.4ms
	auto value = v_us * 4096 / 20000;	//20ms

	SetPWM(ePort, 0, value);
}

DevPCA9685::DevPCA9685() : m_hi2c(nullptr){

}

void DevPCA9685::Write(const uint8_t address, const uint8_t data){
	NULLCHECK;

	uint8_t tx[2] = {address, data};
	HAL_I2C_Master_Transmit(m_hi2c, PCA9685_adrr, tx, 2, 0xffff);
}

uint8_t DevPCA9685::Read(uint8_t address){
	NULLCHECK_RET_U8;

	uint8_t data;

	HAL_I2C_Master_Transmit(m_hi2c, PCA9685_adrr, &address, 1, 0xffff);
	HAL_I2C_Master_Receive(m_hi2c, PCA9685_adrr, &data, 1, 0xffff);

	return data;
}

void DevPCA9685::SetPWMFreq(float freq){
	uint64_t prescale,oldmode,newmode;
	float prescaleval;
	freq *= 0.9;  // Correct for overshoot in the frequency setting
	prescaleval = 25000000;
	prescaleval /= CNT;
	prescaleval /= freq;
	prescaleval -= 1;
	prescale = floor(prescaleval + 0.5);

	oldmode = Read(PCA9685_MODE1);
	newmode = (oldmode&0x7F) | 0x10; // sleep
	Write(PCA9685_MODE1, newmode); // go to sleep
	Write(PCA9685_PRESCALE, prescale); // set the prescaler
	Write(PCA9685_MODE1, oldmode);
	HAL_Delay(5);
	Write(PCA9685_MODE1, oldmode | 0xa1);
}

void DevPCA9685::Reset(){
	Write(PCA9685_MODE1, 0x0);
}

void DevPCA9685::Reset(const double freq){
	Reset();
	HAL_Delay(2);
	SetPWMFreq(freq);
}

void DevPCA9685::Init(double freq/* = 50*/){
	Reset(freq);
}

void DevPCA9685::SetI2CHandle(I2C_HandleTypeDef* i2c){
	m_hi2c = i2c;
}
