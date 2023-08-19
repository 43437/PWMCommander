#include "CMsgParser.h"
#include "Protocol.h"
#include <map>
#include <algorithm>

uint8_t CMsgParser::CRC8Check(const std::vector<uint8_t>& vecData)
{
	uint8_t crc = 0;

	for (auto& dat : vecData)
	{
		crc ^= dat;
		for (int i = 0; i < 8; ++i)
		{
			if ((crc & 0x01) != 0)
			{
				crc >>= 1;
				crc ^= 0x8c;
			}
			else
			{
				crc >>= 1;
			}
		}
	}

	return crc;
}

CMsgParser::CMsgParser() : m_objLoopArray(100)
	, m_funcACK(nullptr)
	, m_funcCMDRecved(nullptr)
{
	Init();
}

void CMsgParser::Cache(std::vector<uint8_t>& vecData)
{
	for (auto& dat : vecData)
	{
		if (!m_objLoopArray.Add(dat))
		{
			//error
		}
	}
}

void CMsgParser::Cache(const uint8_t dat)
{
	if (!m_objLoopArray.Add(dat))
	{
		//error
	}
}

void CMsgParser::Init()
{

}

int CMsgParser::MSGEndDiff(const int paramLen)
{
	return CRCDiff(paramLen) + NSProtocol::MSG_CRC_SIZE;
}

int CMsgParser::CRCDiff(const int paramLen)
{
	const int diff = NSProtocol::MSG_PARAM_DIFF;
	if (paramLen < 0)
	{
		//error len
	}
	else
	{
		return diff + paramLen;
	}
	return diff;
}

void CMsgParser::Test()
{
//	std::vector<std::pair<int, std::vector<uint8_t>>> mpCMD{
//		{0x00, {0x01, 0x02, 0x03}},
//		{0x01, {0x01}}
//	};
//
//	std::vector<uint8_t> vecCMD;
//	uint8_t sn = 0;
//	for (auto& prCMD : mpCMD)
//	{
//		uint8_t cmd = prCMD.first;
//		std::vector<uint8_t>& vecParam = prCMD.second;
//		BuildCMD(cmd, sn, vecParam, vecCMD);
//		Cache(vecCMD);
//	}
//
//	Analyze();
}

void CMsgParser::SkipInvalidData()
{
	const int datSize = m_objLoopArray.DataSize();
	if (datSize >= NSProtocol::MSG_MIN_SIZE)
	{
		int datBegin = 0;
		int datEnd = 0;
		m_objLoopArray.Data(datBegin, datEnd);
		for (int i = 0; i < datSize; ++i)
		{
			auto dat = m_objLoopArray.Data(datBegin);
			if (NSProtocol::MSG_HEAD != dat)
			{
				//skip invalid dat
				datBegin = m_objLoopArray.LoopDistance(datBegin, 1);
				if (datBegin == datEnd)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		m_objLoopArray.ResetIndxFrom(datBegin);
	}
}

void CMsgParser::Analyze()
{
	while (true)
	{
		SkipInvalidData();

		const int datSize = m_objLoopArray.DataSize();
		if (datSize >= NSProtocol::MSG_MIN_SIZE)
		{
			int datBegin = 0;
			int datEnd = 0;
			m_objLoopArray.Data(datBegin, datEnd);

			uint8_t paramLen = m_objLoopArray.Data(datBegin + NSProtocol::MSG_PARAM_LEN_DIFF);

			int msgLen = NSProtocol::MSG_MIN_SIZE + paramLen;
			if (datSize >= msgLen)
			{
				int msgEndDiff = MSGEndDiff(paramLen);
				if (NSProtocol::MSG_END == m_objLoopArray.Data(datBegin + msgEndDiff))
				{
					int msgCRCDiff = CRCDiff(paramLen);
					uint8_t crc = m_objLoopArray.Data(datBegin + msgCRCDiff);

					std::vector<uint8_t> vecDat;
					int lenCMDAndDat = NSProtocol::MSG_CMD_SIZE + paramLen;
					m_objLoopArray.Data(datBegin + NSProtocol::MSG_CMD_DIFF, lenCMDAndDat, vecDat);
					if (CheckCRC(vecDat, crc))
					{
						//analyze success
						uint8_t cmd = vecDat.front();
						uint8_t sn = m_objLoopArray.Data(datBegin + NSProtocol::MSG_SN_DIFF);
						NSProtocol::SCMD stuCMD;
						stuCMD.m_type = m_objLoopArray.Data(datBegin + NSProtocol::MSG_TYPE_DIFF);
						stuCMD.m_sn = sn;
						stuCMD.m_cmd = cmd;
						m_objLoopArray.Data(datBegin + + NSProtocol::MSG_PARAM_DIFF, paramLen, stuCMD.m_vecParam);
						OnRecvCMD(stuCMD);
					}
					else
					{
						//check crc failed.
					}

					//reset current msg dat
					m_objLoopArray.ResetIndxFrom(datBegin + msgLen);
				}
				else
				{
					//error at end, clear all dat
					m_objLoopArray.ResetIndxFrom(datEnd);
				}
			}
			else
			{
				//no enough data
				break;
			}
		}
		else
		{
			//no enough data
			break;
		}
	}
}

bool CMsgParser::CheckCRC(const std::vector<uint8_t>& vecDat, const uint8_t crc)
{
	return crc == CRC8Check(vecDat);
}

void CMsgParser::LoopOnce()
{
	Analyze();
}

void CMsgParser::OnRecvCMD(const NSProtocol::SCMD& stuCMD)
{
	Ack(stuCMD);
	if (nullptr != m_funcCMDRecved)
	{
		m_funcCMDRecved(stuCMD);
	}
}

void CMsgParser::Ack(const NSProtocol::SCMD& stuCMD)
{
	if (nullptr != m_funcACK)
	{
		NSProtocol::SCMD stuAckCMD;
		stuAckCMD.m_type = NSProtocol::TYPE_ACK;
		stuAckCMD.m_sn = stuCMD.m_sn;
		stuAckCMD.m_cmd = stuCMD.m_cmd;
		m_funcACK(stuAckCMD);
	}
}

void CMsgParser::SetCMDRecvHandle(FuncCMDRecved func)
{
	m_funcCMDRecved = func;
}

void CMsgParser::SetCMDACKHandle(FunACK func)
{
	m_funcACK = func;
}
