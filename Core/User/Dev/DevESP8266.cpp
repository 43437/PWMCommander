#include "DevESP8266.h"
#include "stm32f1xx_hal.h"
#include "SEGGER_RTT_V788h/RTT/SEGGER_RTT.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

DevESP8266& DevESP8266::Instance(){
	static DevESP8266 __instance;
	return __instance;
}

DevESP8266::DevESP8266() : m_objLoopArray(LOOP_BUF_SIZE){

}

void DevESP8266::LoopOnce(){


	SendHandle();
}

void DevESP8266::SendHandle(){

	if (!m_dqSendBuf.empty()){
		auto sndDat = m_dqSendBuf.front();
		uint8_t* dat = &sndDat;

		m_dqSendBuf.pop_front();
		int trys = 3;

		while ( HAL_OK != HAL_UART_Transmit(&huart1, dat, 1, 10) && trys > 0){
			--trys;
		}
	}
}

void DevESP8266::Cache(const uint8_t dat){

	m_objLoopArray.Add(dat);
}

void DevESP8266::Reserve(const int datSize){

	int start = 0;
	int cnt = 0;
	m_objLoopArray.DataRange(start, cnt);

	auto diff = cnt;
	if (datSize >= 0) {

		diff -= datSize;
		if (diff < 0){
			diff = 0;
		}
	}

	m_objLoopArray.ResetIndxFrom(start + diff);
}

std::vector<uint8_t> DevESP8266::Read(){

	std::vector<uint8_t> ret;
	int start = 0;
	int cnt = 0;
	m_objLoopArray.DataRange(start, cnt);
	m_objLoopArray.Data(start, cnt, ret);

	return ret;
}

void DevESP8266::Write(const std::vector<uint8_t>& vecDat){

	m_dqSendBuf.insert(m_dqSendBuf.end(), vecDat.begin(), vecDat.end());
}
