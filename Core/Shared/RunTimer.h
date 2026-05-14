#pragma once

//#include "NstCpu.hpp"
#include "pch.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include "Shared/BaseControlManager.h"

		struct TimerCondition
		{
			int address;
			char comparison;
			int value;
		};

		struct Token
		{
			int type;
			int value;
		};

		class RunTimer
		{
		public:
			RunTimer(BaseControlManager* bcm);
			const double GetTime();
			const bool IsValid();
			void UpdateTimer();
			void Reset();
			void DoSetup(BaseControlManager* bcm);

		private:
			int frameCount = 0;
			int timerState = -1; //0=stopped 1=running 2=vpause
			int startFrame = 0;
			int vpauseTime = 0;
			int pauseDelay = 60;
			bool isValid = false;
			BaseControlManager* bcm;
			std::vector<std::vector<TimerCondition>> startConds;
			std::vector<std::vector<TimerCondition>> stopConds;
			std::vector<std::vector<TimerCondition>> vpauseConds;
			std::vector<std::vector<TimerCondition>> resetConds;
			std::vector<std::vector<TimerCondition>> endConds;
			std::vector <std::vector<std::vector<TimerCondition>>*> allConditions = { &startConds,&stopConds,&vpauseConds,&resetConds,&endConds }; //this is great
			std::map<int, int> varMap;
			bool Init();
			bool CheckConditions(std::vector<std::vector<TimerCondition>>& conds);
		};
