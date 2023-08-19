#ifndef __DRV_ESP8266_H_2307172051__
#define __DRV_ESP8266_H_2307172051__

#include <map>
#include <functional>
#include <string>
#include <vector>

extern struct __OP_RET{
	std::vector<uint8_t> OK{'O', 'K'};
	std::vector<uint8_t> ERROR{'E', 'R', 'R', 'O', 'R'};
}OP_RET;

class DrvESP8266{

public:
	enum EInitStep{
		eInitStep_From = 0,
		eInitStep_1 = eInitStep_From,
		eInitStep_2,
		eInitStep_3,
		eInitStep_4,
		eInitStep_5,
		eInitStep_Finished = eInitStep_5,
		eInitStep_To,
	};

	enum EStepStat{
		eStepStat_From,
		eStepStat_Init = eStepStat_From,
		eStepStat_Running,
		eStepStat_Finish,
		eStepStat_Err,
		eStepStat_To,
	};

private:
	struct SInitStepOperation{

		std::function<void()> m_funcOperation;
		std::function<void()> m_funcResultHandle;
	};

	struct SStepRunningInfo{

		uint32_t m_tickstart;
		EStepStat m_eStepStat;
		EInitStep m_eInitStep;
		void Reset();
		SStepRunningInfo();
	};

public:
	static DrvESP8266& Instance();
	void LoopOnce();

	bool Init();
	bool IsRunning();

private:
	DrvESP8266();
	bool CheckFinish();
	void SetRunningFlag(bool bSetFlag);
	void RunningHandle();
	void FinishHandle();
	void Start();
	EStepStat GetStepStat();

	void DoOperate();
	bool CheckOperateOverTime();
	bool CheckOperateFinished(bool& bSuccess);

private:
	std::map<EInitStep, SInitStepOperation> m_mpStepOperation;
	bool m_bRunningFlag;
	SStepRunningInfo m_stuStepRunningInfo;
	const static int KEY_REMAIN_SIZE;
	const static uint32_t OP_OVERTIME;
};

#endif
