#ifndef __MSG_LOOP_20230504_H__
#define __MSG_LOOP_20230504_H__

#include "Msg/CMsgParser.h"
#include <stdint.h>

class MsgLoop
{
public:
	MsgLoop();
	void LoopOnce();
	void InitLoop();

private:
	void Init();

private:
	CMsgParser m_objMsgParser;
};

#endif //__MSG_LOOP_20230504_H__
