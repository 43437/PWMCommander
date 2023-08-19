/*
 * CMDHandler.cpp
 *
 *  Created on: 2023年5月5日
 *      Author: geey17u
 */

#include "CMDHandler.h"
#include "User/Msg/CMsgParser.h"
#include "User/Common/CommonFunction.h"
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1;

CMDHandler& CMDHandler::Instance()
{
	static CMDHandler __instance;
	return __instance;
}

CMDHandler::CMDHandler()
{
	// TODO Auto-generated constructor stub

}

CMDHandler::~CMDHandler()
{
	// TODO Auto-generated destructor stub
}

void CMDHandler::HandleCmd(const NSProtocol::SCMD& stuCMD)
{
	if (m_mpCMDHandle.count(stuCMD.m_cmd) > 0)
	{
		auto& mpHandle = m_mpCMDHandle[stuCMD.m_cmd];
		for (auto itHandle = mpHandle.begin(); mpHandle.end() != itHandle; ++itHandle)
		{
			auto& func = itHandle->second;
			if (func(stuCMD))
			{
				break;
			}
		}
	}
	else
	{
		//unsupport cmd
		NSProtocol::SCMD stuUnSupportCMD = stuCMD;
		stuUnSupportCMD.m_type = NSProtocol::TYPE_INFO;
		stuUnSupportCMD.m_vecParam.clear();
		CommonFunction::SerialTo("N/A", stuUnSupportCMD.m_vecParam);

		SendCMD(stuUnSupportCMD);
	}
}

uint8_t CMDHandler::GetSN()
{
	 static uint8_t sn = 0;
	 return sn++;
}

void CMDHandler::BuildCMD(const uint8_t type, const uint8_t sn, const uint8_t cmd, const std::vector<uint8_t>& vecParam, std::vector<uint8_t>& vecCMD)
{
	const uint8_t paramCnt = vecParam.size();
	const int len = NSProtocol::MSG_MIN_SIZE + paramCnt;
	vecCMD.resize(len);
	vecCMD[NSProtocol::MSG_HEAD_DIFF] = NSProtocol::MSG_HEAD;
	vecCMD[NSProtocol::MSG_TYPE_DIFF] = type;
	vecCMD[NSProtocol::MSG_SN_DIFF] = sn;
	vecCMD[NSProtocol::MSG_PARAM_LEN_DIFF] = paramCnt;
	vecCMD[NSProtocol::MSG_CMD_DIFF] = cmd;
	std::copy(vecParam.begin(), vecParam.end(), vecCMD.begin() + NSProtocol::MSG_PARAM_DIFF);

	uint8_t crcDiff = (uint8_t)CMsgParser::CRCDiff((int)paramCnt);
	std::vector<uint8_t> vecCrcDat;
	vecCrcDat.push_back(cmd);
	vecCrcDat.insert(vecCrcDat.end(), vecParam.begin(), vecParam.end());
	vecCMD[crcDiff] = CMsgParser::CRC8Check(vecCrcDat);

	uint8_t endDiff = (uint8_t)CMsgParser::MSGEndDiff((int)paramCnt);
	vecCMD[endDiff] = NSProtocol::MSG_END;
}

void CMDHandler::BuildCMD(const NSProtocol::SCMD& stuCMD, std::vector<uint8_t>& vecCMD)
{
	BuildCMD(stuCMD.m_type, stuCMD.m_sn, stuCMD.m_cmd, stuCMD.m_vecParam, vecCMD);
}

void CMDHandler::SendCMD(const NSProtocol::SCMD& stuCMD)
{
	std::vector<uint8_t> vecCMD;
	BuildCMD(stuCMD, vecCMD);

	SendCMD(vecCMD);
}

void CMDHandler::SendCMD(std::vector<uint8_t>& vecCMD)
{
	//real send
	for (auto& cmdDat : vecCMD)
	{
		uint8_t* dat = &cmdDat;
		int trys = 3;
		while(HAL_OK != HAL_UART_Transmit(&huart1, dat, 1, 10) && trys > 0)
		{
			--trys;
		}
	}
}

void CMDHandler::RegistCMDHandle(const int handle, uint8_t cmd, FuncCMDHandle func)
{
	m_mpCMDHandle[cmd][handle] = func;
}

void CMDHandler::UnregistCMDHandle(const int handle, uint8_t cmd)
{
	if (m_mpCMDHandle.count(cmd) > 0)
	{
		m_mpCMDHandle[cmd].erase(handle);
	}
}

void CMDHandler::UnregistCMDHandle(const int handle)
{
	for (auto itHandle = m_mpCMDHandle.begin(); m_mpCMDHandle.end() != itHandle; ++itHandle)
	{
		auto cmd = itHandle->first;
		UnregistCMDHandle(handle, cmd);
	}
}
