#ifndef __DEV_ESP8266_H_2307172048__
#define __DEV_ESP8266_H_2307172048__

#include "Msg/LoopArray.h"
#include <stdint.h>
#include <deque>

class DevESP8266{

private:
	enum{
		LOOP_BUF_SIZE = 256,
	};

public:
	static DevESP8266& Instance();
	void LoopOnce();

	void Reserve(const int datSize);

	std::vector<uint8_t> Read();
	void Write(const std::vector<uint8_t>& vecDat);

	void Cache(const uint8_t dat);

private:
	DevESP8266();
	void SendHandle();

private:
	LoopArray m_objLoopArray;
	std::deque<uint8_t> m_dqSendBuf;
};

#endif
