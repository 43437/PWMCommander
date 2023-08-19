/*
 * UARTModule.cpp
 *
 *  Created on: Aug 19, 2023
 *      Author: geey17u
 */

#include <Module/UARTModule.h>
#include "SEGGER_RTT_V788h/RTT/SEGGER_RTT.h"

struct SUSARTBuff{

	SUSARTBuff(USART_TypeDef* uart){
		m_uart = uart;
	};
	USART_TypeDef* m_uart;
	uint8_t m_Buff;
};

static SUSARTBuff USART_DAT[] = {
		SUSARTBuff(USART1),
		SUSARTBuff(USART2),
		SUSARTBuff(USART3),
};

static uint8_t* GetUSARTBuf(USART_TypeDef* uart){

	uint8_t* buf = nullptr;

	auto sz = sizeof(USART_DAT) / sizeof(SUSARTBuff);

	for (auto i = 0; i < sz; ++i){

		auto& stuBuf = USART_DAT[i];
		if (uart == stuBuf.m_uart){
			buf = &stuBuf.m_Buff;
			break;
		}
	}

	return buf;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	auto buf = GetUSARTBuf(huart->Instance);
	if (nullptr != buf){

		UARTModule::Instance().OnUARTData(huart->Instance, *buf);

		SEGGER_RTT_printf(0, "%c", *buf);

		HAL_UART_Receive_IT(huart, buf, 1);
	}
}

UARTModule& UARTModule::Instance(){
	static UARTModule __instance;
	return __instance;
}

UARTModule::UARTModule() {
	// TODO Auto-generated constructor stub

}

UARTModule::~UARTModule() {
	// TODO Auto-generated destructor stub
}

void UARTModule::SetUARTDatHandle(USART_TypeDef* uart, FuncUARTDatHandle func){
	m_mpUARTDatHandle[uart] = func;
}

void UARTModule::StartDataHandle(UART_HandleTypeDef* handle){

	auto buf = GetUSARTBuf(handle->Instance);
	if (nullptr != buf){
		HAL_UART_Receive_IT(handle, buf, 1);
	}
}

void UARTModule::OnUARTData(USART_TypeDef* uart, const uint8_t& dat){

	if (m_mpUARTDatHandle.end() != m_mpUARTDatHandle.find(uart)
			&& nullptr != m_mpUARTDatHandle[uart]){

		m_mpUARTDatHandle[uart](dat);
	}
}
