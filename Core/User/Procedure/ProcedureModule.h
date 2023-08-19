/*
 * ProcedureModule.h
 *
 *  Created on: 2023年5月14日
 *      Author: geey17u
 */

#ifndef USER_PROCEDURE_PROCEDUREMODULE_H_
#define USER_PROCEDURE_PROCEDUREMODULE_H_

class ProcedureModule {
public:
	static ProcedureModule& Instance();
	virtual ~ProcedureModule();

private:
	ProcedureModule();
	void Init();
};

#endif /* USER_PROCEDURE_PROCEDUREMODULE_H_ */
