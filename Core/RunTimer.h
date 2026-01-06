#pragma once

//#include "NstCpu.hpp"
#include "pch.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>

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

		struct ConsoleMemoryInfo2
		{
			void* Memory;
			uint32_t Size;
		};

		class RunTimer
		{
		public:
			RunTimer(void* memory, uint32_t size);
			const double GetTime();
			const bool IsValid();
			void UpdateTimer();
			void Reset();
			void DoSetup(void* memory, uint32_t size);

		private:
			int frameCount;
			int timerState; //0=stopped 1=running 2=vpause
			int startFrame;
			int vpauseTime;
			int pauseDelay;
			bool isValid;
			ConsoleMemoryInfo2 ram;
			std::vector<std::vector<TimerCondition>> startConds;
			std::vector<std::vector<TimerCondition>> stopConds;
			std::vector<std::vector<TimerCondition>> vpauseConds;
			std::vector<std::vector<TimerCondition>> resetConds;
			std::vector<std::vector<TimerCondition>> endConds;
			std::map<int, int> varMap;
			bool Init();
			bool CheckConditions(std::vector<std::vector<TimerCondition>>& conds);
		};
