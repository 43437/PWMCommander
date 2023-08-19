/*
 * UARTModule.h
 *
 *  Created on: Aug 19, 2023
 *      Author: geey17u
 */

#ifndef USER_MODULE_UARTMODULE_H_
#define USER_MODULE_UARTMODULE_H_

#include <functional>
#include <map>
#include <stdint.h>
#include "stm32f1xx_hal.h"

class UARTModule {
public:
	typedef std::function<void(const uint8_t& dat)> FuncUARTDatHandle;

public:
	static UARTModule& Instance();
	virtual ~UARTModule();
	void SetUARTDatHandle(USART_TypeDef* uart, FuncUARTDatHandle func);
	void OnUARTData(USART_TypeDef* uart, const uint8_t& dat);
	void StartDataHandle(UART_HandleTypeDef* handle);

private:
	UARTModule();

private:
	std::map<USART_TypeDef*, FuncUARTDatHandle> m_mpUARTDatHandle;
};

#endif /* USER_MODULE_UARTMODULE_H_ */
