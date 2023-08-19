#include "DrvESP8266.h"
#include "Dev/DevESP8266.h"
#include "stm32f1xx_hal.h"
#include "User/Common/CommonFunction.h"
#include "User/Module/UARTModule.h"
#include "SEGGER_RTT_V788h/RTT/SEGGER_RTT.h"

//1.设置WIFI 模式为 Station：AT+CWMODE=1
//2.连接到家中路由器：AT+CWJAP="2206","19dong2206"
//3.发送AT指令建立 TCP 连接：AT+CIPSTART="TCP","192.168.0.105",8888
//4.开启透传模式：AT+CIPMODE=1
//5.开始透传发送：AT+CIPSEND

struct __OP_RET OP_RET;

const int DrvESP8266::KEY_REMAIN_SIZE = std::max(OP_RET.ERROR.size(), OP_RET.OK.size());
const uint32_t DrvESP8266::OP_OVERTIME = 30000;



void DrvESP8266::SStepRunningInfo::Reset(){

	m_tickstart = 0;
	m_eStepStat = eStepStat_Init;
	m_eInitStep = eInitStep_1;
}

DrvESP8266::SStepRunningInfo::SStepRunningInfo(){

	Reset();
}

DrvESP8266& DrvESP8266::Instance(){
	static DrvESP8266 __instance;

	return __instance;
}

DrvESP8266::DrvESP8266() : m_bRunningFlag(false) {

	auto funcOperate = [=](){

		DoOperate();
	};

	auto funcRetHandle = [=](){

		bool bSuccess = true;
		if (CheckOperateFinished(bSuccess)){

			m_stuStepRunningInfo.m_eStepStat = bSuccess ? eStepStat_Finish : eStepStat_Err;
		}
	};

	m_mpStepOperation = {
			{eInitStep_1, {funcOperate, funcRetHandle}},
			{eInitStep_2, {funcOperate, funcRetHandle}},
			{eInitStep_3, {funcOperate, funcRetHandle}},
			{eInitStep_4, {funcOperate, funcRetHandle}},
			{eInitStep_5, {funcOperate, funcRetHandle}},
	};
}

void DrvESP8266::DoOperate(){

	static std::map<EInitStep, std::vector<uint8_t>> mpOperate{
		{eInitStep_1, {'A','T','+','C','W','M','O','D','E','=','1',0x0D, 0x0A}},
		{eInitStep_2, {'A','T','+','C','W','J','A','P','=','"','2','2','0','6','"',',','"','1','9','d','o','n','g','2','2','0','6','"',0x0D, 0x0A}},
		{eInitStep_3, {'A','T','+','C','I','P','S','T','A','R','T','=','"','T','C','P','"',',','"','1','9','2','.','1','6','8','.','0','.','1','0','5','"',',','8','8','8','8',0x0D, 0x0A}},
		{eInitStep_4, {'A','T','+','C','I','P','M','O','D','E','=','1',0x0D, 0x0A}},
		{eInitStep_5, {'A','T','+','C','I','P','S','E','N','D',0x0D, 0x0A}},
	};

	//send cmd;
	auto& vecCMD = mpOperate[m_stuStepRunningInfo.m_eInitStep];

	//real send
	DevESP8266::Instance().Write(vecCMD);

	m_stuStepRunningInfo.m_eStepStat = eStepStat_Running;
	//update tick start
	m_stuStepRunningInfo.m_tickstart = HAL_GetTick();
}

bool DrvESP8266::CheckOperateFinished(bool& bSuccess){

	bool bFinished = false;

	auto vecDat = DevESP8266::Instance().Read();

	auto reservDat = KEY_REMAIN_SIZE;
	if (CommonFunction::Contains(vecDat, OP_RET.OK)){

		bSuccess = true;
		bFinished = true;
		reservDat = 0;
	}else if(CommonFunction::Contains(vecDat, OP_RET.ERROR)){

		bSuccess = false;
		bFinished = true;
		reservDat = 0;
	}

	DevESP8266::Instance().Reserve(reservDat);

	return bFinished;
}

bool DrvESP8266::CheckOperateOverTime(){

	auto bOverTime = (CommonFunction::Duration(m_stuStepRunningInfo.m_tickstart, HAL_GetTick()) > OP_OVERTIME);
	return bOverTime;
}

bool DrvESP8266::CheckFinish(){

	bool bFinished = false;

	switch (m_stuStepRunningInfo.m_eStepStat){
		case eStepStat_Init:

			break;
		case eStepStat_Running:
			if (CheckOperateOverTime()){
				bFinished = true;
				m_stuStepRunningInfo.m_eStepStat = eStepStat_Err;
			}
			break;
		case eStepStat_Finish:
		{
			bFinished = (m_stuStepRunningInfo.m_eInitStep >= eInitStep_Finished);
		}
			break;
		case eStepStat_Err:
			bFinished = true;
			break;
		default:
			break;
		}

	return bFinished;
}

void DrvESP8266::RunningHandle(){

	auto& step = m_stuStepRunningInfo.m_eInitStep;
	switch (m_stuStepRunningInfo.m_eStepStat){
	case eStepStat_Init:
		m_mpStepOperation[step].m_funcOperation();
		break;
	case eStepStat_Running:
		m_mpStepOperation[step].m_funcResultHandle();
		break;
	case eStepStat_Finish:
		if (step < eInitStep_Finished){
			step = EInitStep(step + 1);
			m_stuStepRunningInfo.m_eStepStat = eStepStat_Init;
		}
		break;
	case eStepStat_Err:
		//nothing TODO
		break;
	default:
		break;
	}
}

void DrvESP8266::FinishHandle(){

	SetRunningFlag(false);
	UARTModule::Instance().SetUARTDatHandle(USART1, nullptr);
}

void DrvESP8266::LoopOnce(){

	if (IsRunning()){

		DevESP8266::Instance().LoopOnce();

		if (CheckFinish()){

			FinishHandle();
		}else{

			RunningHandle();
		}
	}
}

bool DrvESP8266::Init(){

	bool bRet = true;
	if (!IsRunning()){

		Start();
		while(true){
			if (IsRunning()){
				LoopOnce();
			}else{
				bRet = (eStepStat_Finish == GetStepStat());
				break;
			}
		}
	}
	return bRet;
}

DrvESP8266::EStepStat DrvESP8266::GetStepStat(){

	return m_stuStepRunningInfo.m_eStepStat;
}

bool DrvESP8266::IsRunning(){

	return m_bRunningFlag;
}

void DrvESP8266::Start(){

	if (!IsRunning()){
		SetRunningFlag(true);
		m_stuStepRunningInfo.Reset();

		UARTModule::Instance().SetUARTDatHandle(USART1, [=](const uint8_t& dat){

			if (DrvESP8266::Instance().IsRunning()){

				DevESP8266::Instance().Cache(dat);
			}
		});
	}
}

void DrvESP8266::SetRunningFlag(bool bSetFlag){
	m_bRunningFlag = bSetFlag;
}
