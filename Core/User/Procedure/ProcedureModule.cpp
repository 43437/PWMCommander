/*
 * ProcedureModule.cpp
 *
 *  Created on: 2023年5月14日
 *      Author: geey17u
 */

#include "User/Procedure/ProcedureModule.h"
#include "User/Procedure/InfoProcedure.h"
#include "User/Procedure/PWMProcedure.h"

ProcedureModule::ProcedureModule() {
	// TODO Auto-generated constructor stub
	Init();
}

ProcedureModule::~ProcedureModule() {
	// TODO Auto-generated destructor stub
}

ProcedureModule& ProcedureModule::Instance(){
	static ProcedureModule __instance;
	return __instance;
}

void ProcedureModule::Init()
{
	InfoProcedure::Instance();
	PWMProcedure::Instance();
}
