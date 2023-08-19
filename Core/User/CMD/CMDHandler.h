/*
 * CMDHandler.h
 *
 *  Created on: 2023年5月5日
 *      Author: geey17u
 */

#ifndef USER_CMD_CMDHANDLER_H_
#define USER_CMD_CMDHANDLER_H_

#include "User/Msg/Protocol.h"
#include <functional>
#include <map>

#define CMDHandlerInstance CMDHandler::Instance()

class CMDHandler
{
public:
	typedef std::function<bool(const NSProtocol::SCMD& stuCMD)> FuncCMDHandle;

public:
	static CMDHandler& Instance();
	virtual ~CMDHandler();

	void HandleCmd(const NSProtocol::SCMD& stuCMD);
	void SendCMD(const NSProtocol::SCMD& stuCMD);

	static void BuildCMD(const uint8_t type, const uint8_t sn, const uint8_t cmd, const std::vector<uint8_t>& vecParam, std::vector<uint8_t>& vecCMD);
	static void BuildCMD(const NSProtocol::SCMD& stuCMD, std::vector<uint8_t>& vecCMD);
	static uint8_t GetSN();

	void RegistCMDHandle(const int handle, uint8_t cmd, FuncCMDHandle func);
	void UnregistCMDHandle(const int handle, uint8_t cmd);
	void UnregistCMDHandle(const int handle);

private:
	CMDHandler();
	void SendCMD(std::vector<uint8_t>& vecCMD);

private:
	std::map<uint8_t, std::map<int, FuncCMDHandle>> m_mpCMDHandle;
};

#endif /* USER_CMD_CMDHANDLER_H_ */
