//version 2307102123
#pragma once

#include <stdint.h>
#include <vector>

namespace NSProtocol
{
	//size
	const int MSG_HEAD_SIZE = 1;
	const int MSG_TYPE_SIZE = 1;
	const int MSG_SN_SIZE = 1;
	const int MSG_PARAM_LEN_SIZE = 1;
	const int MSG_CMD_SIZE = 1;
	const int MSG_CRC_SIZE = 1;
	const int MSG_END_SIZE = 1;

	const int MSG_MIN_SIZE = MSG_HEAD_SIZE + MSG_TYPE_SIZE + MSG_SN_SIZE + MSG_PARAM_LEN_SIZE + MSG_CMD_SIZE + MSG_CRC_SIZE + MSG_END_SIZE;

	//filed magic
	const uint8_t MSG_HEAD = 0xFC;
	const uint8_t MSG_END = 0xFD;

	//filed diff
	const int MSG_HEAD_DIFF = 0;
	const int MSG_TYPE_DIFF = MSG_HEAD_DIFF + MSG_HEAD_SIZE;
	const int MSG_SN_DIFF = MSG_TYPE_DIFF + MSG_TYPE_SIZE;
	const int MSG_PARAM_LEN_DIFF = MSG_SN_DIFF + MSG_SN_SIZE;
	const int MSG_CMD_DIFF = MSG_PARAM_LEN_DIFF + MSG_PARAM_LEN_SIZE;
	const int MSG_PARAM_DIFF = MSG_CMD_DIFF + MSG_CMD_SIZE;

	//cmd type
	const uint8_t TYPE_CMD = 0;
	const uint8_t TYPE_ACK = 1;
	const uint8_t TYPE_RET = 2;
	const uint8_t TYPE_INFO = 3;

	struct SCMD
	{
		uint8_t m_type;
		uint8_t m_sn;
		uint8_t m_cmd;
		std::vector<uint8_t> m_vecParam;
	};

	const struct __CMDID
	{
        const uint8_t ECHO = 0x00;          //param(0): null
        const uint8_t START_PWM = 0x10;     //param(0): null
        const uint8_t STOP_PWM = 0x11;      //param(0): null
        const uint8_t PWM_PERCENT = 0x12;   //param(1): 00-64
        const uint8_t PWM_DEGREE = 0x13;    //param(1): 00-B4
        const uint8_t PWM_N_PERCENT = 0x14;	//param(2): port(00-0F) percent(00-64)
        const uint8_t PWM_N_DEGREE = 0x15;	//param(2): port(00-0F) degree(00-B4)
	}CMDID;
};
