/*
 * PWMProcedure.cpp
 *
 *  Created on: 2023年5月14日
 *      Author: geey17u
 */

#include "User/Procedure/PWMProcedure.h"
#include "User/CMD/CMDHandler.h"
#include "User/Msg/Protocol.h"
#include "stm32f1xx_hal.h"
#include "User/Common/CommonFunction.h"
#include "User/Config.h"
#include "User/Dev/DevPCA9685.h"
#include <string>

#ifndef PCA9685
extern TIM_HandleTypeDef htim4;
#endif

PWMProcedure::PWMProcedure() {
	// TODO Auto-generated constructor stub
	Init();
}

PWMProcedure::~PWMProcedure() {
	// TODO Auto-generated destructor stub
}

PWMProcedure& PWMProcedure::Instance()
{
	static PWMProcedure __instance;
	return __instance;
}

void PWMProcedure::Init()
{
	using NSProtocol::CMDID;

#ifndef PCA9685
	CMDHandlerInstance.RegistCMDHandle(0, CMDID.START_PWM, [](const NSProtocol::SCMD& stuCMD){

		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

		NSProtocol::SCMD stuRetCMD = stuCMD;
		stuRetCMD.m_type = NSProtocol::TYPE_RET;
		CMDHandlerInstance.SendCMD(stuRetCMD);
		return true;
	});

	CMDHandlerInstance.RegistCMDHandle(0, CMDID.STOP_PWM, [](const NSProtocol::SCMD& stuCMD){

		HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);

		NSProtocol::SCMD stuRetCMD = stuCMD;
		stuRetCMD.m_type = NSProtocol::TYPE_RET;
		CMDHandlerInstance.SendCMD(stuRetCMD);
		return true;
	});

	CMDHandlerInstance.RegistCMDHandle(0, CMDID.PWM_PERCENT, [](const NSProtocol::SCMD& stuCMD){

		bool bParamCheck = true;
		int cnt = 0;
		if (stuCMD.m_vecParam.size() > 0)
		{
			auto& percent = stuCMD.m_vecParam[0];
			if (percent >= 0 && percent <= 100)
			{
				cnt = 20000 / 100 * percent;
			}
			else
			{
				bParamCheck = false;
			}
		}
		else
		{
			bParamCheck = false;
		}

		if (bParamCheck)
		{
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, (uint32_t)cnt);

			NSProtocol::SCMD stuRetCMD = stuCMD;
			stuRetCMD.m_type = NSProtocol::TYPE_RET;
			CMDHandlerInstance.SendCMD(stuRetCMD);
		}
		else
		{
			NSProtocol::SCMD stuRetCMD = stuCMD;
			stuRetCMD.m_type = NSProtocol::TYPE_INFO;
			stuRetCMD.m_vecParam.clear();
			CommonFunction::SerialTo("param error", stuRetCMD.m_vecParam);
			CMDHandlerInstance.SendCMD(stuRetCMD);
		}


		return true;
	});

	CMDHandlerInstance.RegistCMDHandle(0, CMDID.PWM_DEGREE, [](const NSProtocol::SCMD& stuCMD){

		const static int DEGREE_MIN = 0;
		const static int DEGREE_MAX = 180;
		const static int CNT_MIN = 500;
		const static int CNT_RANG = 2000;
		bool bParamCheck = true;
		int cnt = 0;
		if (stuCMD.m_vecParam.size() > 0)
		{
			auto& degree = stuCMD.m_vecParam[0];
			if (degree >= DEGREE_MIN && degree <= DEGREE_MAX)
			{
				float percent = (float)degree / (float)DEGREE_MAX;
				cnt = (float)CNT_MIN + percent * (float)CNT_RANG; 		//from 500 to 2000 is behalf 0 to 180 degree;
			}
			else
			{
				bParamCheck = false;
			}
		}
		else
		{
			bParamCheck = false;
		}

		if (bParamCheck)
		{
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, (uint32_t)cnt);

			NSProtocol::SCMD stuRetCMD = stuCMD;
			stuRetCMD.m_type = NSProtocol::TYPE_RET;
			CMDHandlerInstance.SendCMD(stuRetCMD);
		}
		else
		{
			NSProtocol::SCMD stuRetCMD = stuCMD;
			stuRetCMD.m_type = NSProtocol::TYPE_INFO;
			stuRetCMD.m_vecParam.clear();
			CommonFunction::SerialTo("param error", stuRetCMD.m_vecParam);
			CMDHandlerInstance.SendCMD(stuRetCMD);
		}

		return true;
	});

#else
	CMDHandlerInstance.RegistCMDHandle(0, CMDID.PWM_N_PERCENT, [](const NSProtocol::SCMD& stuCMD){

		bool bParamCheck = true;

		uint8_t port = 0;
		uint8_t percent = 0;
		if (stuCMD.m_vecParam.size() > 1)
		{
			port = stuCMD.m_vecParam[0];
			percent = stuCMD.m_vecParam[1];

			if (! (percent >= 0 && percent <= 100
					&& port >= DevPCA9685::ePort_0 && port <= DevPCA9685::ePort_End) ){
				bParamCheck = false;
			}
		}
		else
		{
			bParamCheck = false;
		}

		if (bParamCheck){

			DevPCA9685::Instance().SetPWMPercent((DevPCA9685::EPort)port, percent);

			NSProtocol::SCMD stuRetCMD = stuCMD;
			stuRetCMD.m_type = NSProtocol::TYPE_RET;
			CMDHandlerInstance.SendCMD(stuRetCMD);
		}else{
			NSProtocol::SCMD stuRetCMD = stuCMD;
			stuRetCMD.m_type = NSProtocol::TYPE_INFO;
			stuRetCMD.m_vecParam.clear();
			CommonFunction::SerialTo("param error", stuRetCMD.m_vecParam);
			CMDHandlerInstance.SendCMD(stuRetCMD);
		}

		return true;
	});

	CMDHandlerInstance.RegistCMDHandle(0, CMDID.PWM_N_DEGREE, [](const NSProtocol::SCMD& stuCMD){

		const static int DEGREE_MIN = 0;
		const static int DEGREE_MAX = 180;

		bool bParamCheck = true;
		uint8_t degree = 0;
		uint8_t port = 0;

		if (stuCMD.m_vecParam.size() > 1)
		{
			port = stuCMD.m_vecParam[0];
			degree = stuCMD.m_vecParam[1];

			if (! (degree >= DEGREE_MIN && degree <= DEGREE_MAX
					&& port >= DevPCA9685::ePort_0 && port <= DevPCA9685::ePort_End) )
			{
				bParamCheck = false;
			}
		}
		else
		{
			bParamCheck = false;
		}

		if (bParamCheck)
		{
			DevPCA9685::Instance().SetPWMDegree((DevPCA9685::EPort)port, degree);

			NSProtocol::SCMD stuRetCMD = stuCMD;
			stuRetCMD.m_type = NSProtocol::TYPE_RET;
			CMDHandlerInstance.SendCMD(stuRetCMD);
		}
		else
		{
			NSProtocol::SCMD stuRetCMD = stuCMD;
			stuRetCMD.m_type = NSProtocol::TYPE_INFO;
			stuRetCMD.m_vecParam.clear();
			CommonFunction::SerialTo("param error", stuRetCMD.m_vecParam);
			CMDHandlerInstance.SendCMD(stuRetCMD);
		}

		return true;
	});
#endif
}
