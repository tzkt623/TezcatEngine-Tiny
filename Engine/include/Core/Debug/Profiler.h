#pragma once

/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/CppHead.h"

namespace tezcat::Tiny
{
	struct TINY_API Profiler
	{
		static const unsigned char* GPU;

		static int PassCount;
		static int DrawCall;
		static float2 mouseOffset;
		static float2 mousePosition;

		static double RenderTime;
		static double LogicTime;

		static size_t MemoryAlloc;
		static size_t MemoryFree;

		static size_t getMemoryUse() { return MemoryAlloc; }
	};

	class TINY_API ProfilerTimer
	{
	public:
		ProfilerTimer();
		ProfilerTimer(const char* functionName);
		ProfilerTimer(double& deltaTime);
		ProfilerTimer(const char* functionName, double& deltaTime);
		~ProfilerTimer();

		void stop();
		double stopOut();
		const char* getName() const { return mName; }

	private:
		const char* mName;
		double& mDeltaTime;
		bool mNoStop;
		std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;

	public:
		static double DefaultDeltaTime;
	};

#ifndef TINY_RELEASE
#define TINY_PROFILER_RESET_DRAWCALL() Profiler::DrawCall = 0
#define TINY_PROFILER_RESET_PASSCOUNT() Profiler::PassCount = 0
#define TINY_PROFILER_DRAWCALL(x) Profiler::DrawCall += x
#define TINY_PROFILER_PASSCOUNT(x) Profiler::PassCount += x

#define TINY_PROFILER_TIMER() ProfilerTimer PROFILER_Timer##__LINE__
#define TINY_PROFILER_TIMER_OUT(deltaTime) ProfilerTimer PROFILER_Timer_Out##__LINE__(deltaTime)
#define TINY_PROFILER_TIMER_FUNCTION() ProfilerTimer PROFILER_Timer##__LINE__(__FUNCTION__)
#define TINY_PROFILER_TIMER_FUNCTION_OUT(deltaTime) ProfilerTimer PROFILER_Timer##__LINE__(__FUNCTION__, deltaTime)

#define TINY_PROFILER_TIMER_NAME() ProfilerTimer PROFILER_Timer##__FUNCTION__
#define TINY_PROFILER_TIMER_NAME_OUT(out) out = PROFILER_Timer##__FUNCTION__.stopOut()
#define TINY_PROFILER_TIMER_NAME_LOG() std::cout << PROFILER_Timer##__FUNCTION__.stopOut() << std::endl

#define TINY_PROFILER_TIMER_LOG() std::cout << ProfilerTimer::DefaultDeltaTime << std::endl
#define TINY_PROFILER_TIMER_LOG_OUT(deltaTime) std::cout << deltaTime * 0.0001 << std::endl
#define TINY_PROFILER_TIMER_FUNCTION_LOG() std::cout << PROFILER_Timer##__LINE__.getName() << ": " << ProfilerTimer::DefaultDeltaTime << std::endl

#else
#define TINY_PROFILER_RESET_DRAWCALL()
#define TINY_PROFILER_RESET_PASSCOUNT()
#define TINY_PROFILER_DRAWCALL(x)
#define TINY_PROFILER_PASSCOUNT(x)

#define TINY_PROFILER_TIMER()
#define TINY_PROFILER_TIMER_OUT(deltaTime)
#define TINY_PROFILER_TIMER_FUNCTION()
#define TINY_PROFILER_TIMER_FUNCTION_OUT(deltaTime)

#define TINY_PROFILER_LOG_TIME()
#endif
}
