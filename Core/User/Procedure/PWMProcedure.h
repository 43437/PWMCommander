/*
 * PWMProcedure.h
 *
 *  Created on: 2023年5月14日
 *      Author: geey17u
 */

#ifndef USER_PROCEDURE_PWMPROCEDURE_H_
#define USER_PROCEDURE_PWMPROCEDURE_H_

class PWMProcedure {
public:
	static PWMProcedure& Instance();
	virtual ~PWMProcedure();

private:
	PWMProcedure();
	void Init();
};

#endif /* USER_PROCEDURE_PWMPROCEDURE_H_ */
