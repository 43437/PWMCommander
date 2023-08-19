#pragma once

#include <vector>
#include <stdint.h>

class LoopArray
{
public:
	LoopArray(const int sz);

	int DataSize() const;
	void Remove(const int cnt);
	bool Add(uint8_t dat);
	void ResetIndxFrom(const int indxFrom);
	int LoopDistance(const int start, const int distance) const;
	int Distance(const int start, const int end) const;
	int AvaliableSize() const;

	const std::vector<uint8_t>& Data(int& start, int& end);

	uint8_t Data(const int index) const;
	void Data(const int index, const int len, std::vector<uint8_t>&) const;
	void DataRange(int& start, int& cnt);

	inline int Index(const int index) const;

private:
	void Init();

private:
	std::vector<uint8_t> m_arrData;
	int m_size;
	int m_datIndxFrom;
	int m_datIndxto;
};

