/*
 * CommonFunction.h
 *
 *  Created on: 2023年5月14日
 *      Author: geey17u
 */

#ifndef USER_COMMON_COMMONFUNCTION_H_
#define USER_COMMON_COMMONFUNCTION_H_

#include <string>
#include <vector>

namespace CommonFunction {

	void SerialTo(const std::string& info, std::vector<uint8_t>& vecInfo);
	uint32_t Duration(uint32_t from, uint32_t to);

	template<typename T>
	bool Contains(const std::vector<T>& vecRegion, const std::vector<T>& vecMatch){

		bool bRet = false;
		const auto lenMatch = vecMatch.size();
		const auto lenRegion = vecRegion.size();

		if (0 == lenMatch){

			bRet = true;
		}else if(lenRegion >= lenMatch){

			for (auto i = 0; i <= lenRegion - lenMatch; ++i){

				auto bMatchAll = true;
				for (auto j = 0; j < lenMatch; ++j){

					if (vecRegion[j+i] != vecMatch[j]){
						bMatchAll = false;
					}
				}

				if (bMatchAll){
					bRet = true;
					break;
				}
			}
		}else{
			bRet = false;
		}

		return bRet;
	}
};

#endif /* USER_COMMON_COMMONFUNCTION_H_ */
