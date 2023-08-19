#include "LoopArray.h"

LoopArray::LoopArray(const int sz) : m_size(sz)
, m_datIndxFrom(0)
, m_datIndxto(m_datIndxFrom)
{
	Init();
}

void LoopArray::Init()
{
	m_arrData.resize(m_size, 0);
}

// [m_datIndxFrom, m_datIndxto)
int LoopArray::DataSize() const
{
	return Distance(m_datIndxFrom, m_datIndxto);
}

int LoopArray::Distance(const int start, const int end) const
{
	int iRet = -1;
	if (end >= start)
	{
		iRet = end - start;
	}
	else
	{
		iRet = m_size - start + end;
	}
	return iRet;
}

int LoopArray::LoopDistance(const int start, const int distance) const
{
	return ((((start % m_size) + m_size + distance) % m_size) + m_size) % m_size;
}

void LoopArray::Remove(const int cnt)
{
	if (cnt > 0)
	{
		int sz = DataSize();
		if (sz > cnt)
		{
			m_datIndxFrom = LoopDistance(m_datIndxFrom, cnt);
		}
		else
		{
			m_datIndxFrom = m_datIndxto;
		}
	}
	else
	{
		//error
	}
}

bool LoopArray::Add(uint8_t dat)
{
	bool bRet = false;
	if (AvaliableSize() > 0)
	{
		m_arrData[m_datIndxto] = dat;
		m_datIndxto = LoopDistance(m_datIndxto, 1);
		bRet = true;
	}
	return bRet;
}

void LoopArray::ResetIndxFrom(const int indxFrom)
{
	m_datIndxFrom = Index(indxFrom);
}

int LoopArray::AvaliableSize() const
{
	return m_size - DataSize() - 1;
}

const std::vector<uint8_t>& LoopArray::Data(int& start, int& end)
{
	start = m_datIndxFrom;
	end = m_datIndxto;
	return m_arrData;
}

uint8_t LoopArray::Data(const int index) const
{
	int indx = Index(index);
	if (indx >= 0 && indx < m_size)
	{
		return m_arrData[indx];
	}
	else
	{
		//exception
		return 0x00;
	}
}

void LoopArray::Data(const int index, const int len, std::vector<uint8_t>& vecDat) const
{
	for (int i = 0; i < len; ++i){
		vecDat.push_back(Data(index + i));
	}
}

void LoopArray::DataRange(int& start, int& cnt){

	start = m_datIndxFrom;
	cnt = Distance(m_datIndxFrom, m_datIndxto);
}

int LoopArray::Index(const int index) const
{
	return ((index % m_size) + m_size) % m_size;
}
