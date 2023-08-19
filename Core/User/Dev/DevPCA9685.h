#ifndef __PWMPCA9685_20210206191056_H__
#define __PWMPCA9685_20210206191056_H__

#include <stdint.h>
#include "stm32f1xx_hal.h"

class DevPCA9685 {

public:
	enum EPort{
		ePort_From = 0,
		ePort_0 = ePort_From,
		ePort_1,
		ePort_2,
		ePort_3,
		ePort_4,
		ePort_5,
		ePort_6,
		ePort_7,
		ePort_8,
		ePort_9,
		ePort_10,
		ePort_11,
		ePort_End,
	};
public:
	static DevPCA9685& Instance();
	void SetPWM(const EPort ePort, uint16_t on, uint16_t off);
	void SetPWMPercent(const EPort ePort, uint8_t percent);
	void SetPWMDegree(const EPort ePort, uint8_t degree);
	void Reset(const double freq);
	void Init(double freq = 50);
	void SetI2CHandle(I2C_HandleTypeDef* i2c);

private:
	DevPCA9685();
	void Reset();
	void SetPWMFreq(float freq);

	void Write(const uint8_t address, const uint8_t data);
	uint8_t Read(uint8_t address);

private:
	enum {
		PCA9685_adrr = 0x80,
		PCA9685_MODE1 = 0x0,
		PCA9685_PRESCALE = 0xFE,
		LED0_ON_L = 0x6,
		LED0_ON_H = 0x7,
		LED0_OFF_L = 0x8,
		LED0_OFF_H = 0x9,
		CNT = 4096,
	};

private:
	I2C_HandleTypeDef *m_hi2c;
};

#endif	//__PWMPCA9685_20210206191056_H__
