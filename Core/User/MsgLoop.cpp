#include "MsgLoop.h"
#include "User/CMD/CMDHandler.h"
#include "User/Module/UARTModule.h"

MsgLoop::MsgLoop()
{
	Init();
}

void MsgLoop::Init()
{
	m_objMsgParser.SetCMDACKHandle([](const NSProtocol::SCMD& stuCMD){
		CMDHandlerInstance.SendCMD(stuCMD);
	});

	m_objMsgParser.SetCMDRecvHandle([](const NSProtocol::SCMD& stuCMD){
		CMDHandlerInstance.HandleCmd(stuCMD);
	});
}

void MsgLoop::InitLoop(){
	UARTModule::Instance().SetUARTDatHandle(USART1, [=](const uint8_t& dat){

		m_objMsgParser.Cache(dat);
	});
}

void MsgLoop::LoopOnce()
{
	m_objMsgParser.LoopOnce();
}
