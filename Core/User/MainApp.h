/*
 * MainApp.h
 *
 *  Created on: Aug 19, 2023
 *      Author: geey17u
 */

#ifndef USER_MAINAPP_H_
#define USER_MAINAPP_H_

#include "User/Config.h"
#include "User/MsgLoop.h"

class MainApp {
public:
	MainApp();
	virtual ~MainApp();

	void LoopOnce();

private:
	void Init();

private:
	MsgLoop m_objMsgLoop;
};

#endif /* USER_MAINAPP_H_ */
