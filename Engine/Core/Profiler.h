#pragma once
#include "Head/GLMHead.h"
#include "Head/ConfigHead.h"
#include "Head/CppHead.h"

namespace tezcat::Tiny
{
	struct TINY_API Profiler
	{
		static const unsigned char* GPU;

		static int PassCount;
		static int DrawCall;
		static glm::vec2 mouseOffset;
		static glm::vec2 mousePosition;

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

#ifndef TINY_Release
#define Profiler_ResetDrawCall() Profiler::DrawCall = 0
#define Profiler_ResetPassCount() Profiler::PassCount = 0
#define Profiler_DrawCall(x) Profiler::DrawCall += x
#define Profiler_PassCount(x) Profiler::PassCount += x

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
#define Profiler_ResetDrawCall()
#define Profiler_ResetPassCount()
#define Profiler_DrawCall(x)
#define Profiler_PassCount(x)

#define TINY_PROFILER_TIMER()
#define TINY_PROFILER_TIMER_OUT(deltaTime)
#define TINY_PROFILER_TIMER_FUNCTION()
#define TINY_PROFILER_TIMER_FUNCTION_OUT(deltaTime)

#define TINY_PROFILER_LOG_TIME()
#endif
}
