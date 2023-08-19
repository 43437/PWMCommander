#pragma once

#include <array>
#include <functional>
#include "LoopArray.h"
#include "Protocol.h"

class CMsgParser
{
public:
	typedef std::function<void(const NSProtocol::SCMD& stuCMD)> FunACK;
	typedef std::function<void(const NSProtocol::SCMD& stuCMD)> FuncCMDRecved;

public:
	CMsgParser();
	void Cache(std::vector<uint8_t>& vecData);
	void Cache(const uint8_t dat);

	static uint8_t CRC8Check(const std::vector<uint8_t>& vecData);
	static int MSGEndDiff(const int paramLen);
	static int CRCDiff(const int paramLen);
	static bool CheckCRC(const std::vector<uint8_t>& vecDat, const uint8_t crc);

	void Test();

	void LoopOnce();

	void SetCMDRecvHandle(FuncCMDRecved func);
	void SetCMDACKHandle(FunACK func);

private:
	void Init();
	void Analyze();
	void SkipInvalidData();
	void OnRecvCMD(const NSProtocol::SCMD& stuCMD);
	void Ack(const NSProtocol::SCMD& stuCMD);

private:
	LoopArray m_objLoopArray;
	FunACK m_funcACK;
	FuncCMDRecved m_funcCMDRecved;
};

