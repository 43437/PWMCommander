/*
 * CommonFunction.cpp
 *
 *  Created on: 2023年5月14日
 *      Author: geey17u
 */

#include <User/Common/CommonFunction.h>
#include <algorithm>
#include <limits>

namespace CommonFunction {

	void SerialTo(const std::string& info, std::vector<uint8_t>& vecInfo){
		vecInfo.insert(vecInfo.end(), info.begin(), info.end());
	}

	uint32_t Duration(uint32_t from, uint32_t to){
		if (to >= from){
			return to - from;
		}else{
			return std::numeric_limits<uint32_t>::max() - from + to;
		}
	}
};
