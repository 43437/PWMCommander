/*
 * InfoProcedure.h
 *
 *  Created on: 2023年5月6日
 *      Author: geey17u
 */

#ifndef USER_PROCEDURE_INFOPROCEDURE_H_
#define USER_PROCEDURE_INFOPROCEDURE_H_

class InfoProcedure {
public:
	static InfoProcedure& Instance();
	virtual ~InfoProcedure();

private:
	InfoProcedure();
	void Init();
};

#endif /* USER_PROCEDURE_INFOPROCEDURE_H_ */
