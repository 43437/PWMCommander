/*
 * MainApp.cpp
 *
 *  Created on: Aug 19, 2023
 *      Author: geey17u
 */

#include <MainApp.h>
#include "User/Procedure/ProcedureModule.h"
#include "User/Driver/DrvESP8266.h"
#include "User/Driver/DrvPCA9685.h"
#include "stm32f1xx_hal.h"
#include "User/Module/UARTModule.h"

extern UART_HandleTypeDef huart1;

MainApp::MainApp() {
	// TODO Auto-generated constructor stub
	Init();
}

MainApp::~MainApp() {
	// TODO Auto-generated destructor stub
}

void MainApp::Init(){

	//module init
	UARTModule::Instance().StartDataHandle(&huart1);
	//module init end

	//driver init
	auto ret = DrvESP8266::Instance().Init();
	SEGGER_RTT_printf(0, "init: %d", ret);

	DrvPCA9685::Instance();
	//driver init end

	//procedure init
	ProcedureModule::Instance();
	//procedure init end

	//msgloop init
	m_objMsgLoop.InitLoop();
	//msgloop init end
}

void MainApp::LoopOnce(){

	m_objMsgLoop.LoopOnce();
}
