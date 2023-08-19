/*
 * InfoProcedure.cpp
 *
 *  Created on: 2023年5月6日
 *      Author: geey17u
 */

#include "User/Procedure/InfoProcedure.h"
#include "User/CMD/CMDHandler.h"
#include "User/Msg/Protocol.h"

InfoProcedure& InfoProcedure::Instance()
{
	static InfoProcedure __instance;
	return __instance;
}

InfoProcedure::InfoProcedure() {
	// TODO Auto-generated constructor stub
	Init();
}

void InfoProcedure::Init()
{
	using NSProtocol::CMDID;
	CMDHandlerInstance.RegistCMDHandle(0, CMDID.ECHO, [](const NSProtocol::SCMD& stuCMD){

		NSProtocol::SCMD stuRetCMD = stuCMD;
		stuRetCMD.m_type = NSProtocol::TYPE_RET;
		CMDHandlerInstance.SendCMD(stuRetCMD);
		return true;
	});
}

InfoProcedure::~InfoProcedure() {
	// TODO Auto-generated destructor stub
}

