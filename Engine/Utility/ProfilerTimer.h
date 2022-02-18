#pragma once

#include "../Core/Head/CppHead.h"

namespace tezcat::Tiny::Utility
{
	class ProfilerTimer
	{
	public:
		ProfilerTimer();
		ProfilerTimer(const char* functionName);
		ProfilerTimer(double& deltaTime);
		ProfilerTimer(const char* functionName, double& deltaTime);
		~ProfilerTimer();

		void stop();
		void stop(double& time);
		const char* getName() const { return m_Name; }

	private:
		const char* m_Name;
		double& m_DeltaTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

	public:
		static double DefaultDeltaTime;
	};
}


#ifdef TINY_DEBUG
#define TINY_PROFILER_TIMER() ProfilerTimer PROFILER_Timer##__LINE__
#define TINY_PROFILER_TIMER_OUT(deltaTime) ProfilerTimer PROFILER_Timer_Out##__LINE__(deltaTime)
#define TINY_PROFILER_TIMER_FUNCTION() ProfilerTimer PROFILER_Timer##__LINE__(__FUNCTION__)
#define TINY_PROFILER_TIMER_FUNCTION_OUT(deltaTime) ProfilerTimer PROFILER_Timer##__LINE__(__FUNCTION__, deltaTime)


#define TINY_PROFILER_TIMER_LOG() std::cout << ProfilerTimer::DefaultDeltaTime << std::endl
#define TINY_PROFILER_TIMER_LOG_OUT(deltaTime) std::cout << deltaTime << std::endl
#define TINY_PROFILER_TIMER_FUNCTION_LOG() std::cout << PROFILER_Timer##__LINE__.getName() << ": " << ProfilerTimer::DefaultDeltaTime << std::endl
#else
#define TINY_PROFILER_TIMER()
#define TINY_PROFILER_TIMER_OUT(deltaTime)
#define TINY_PROFILER_TIMER_FUNCTION()
#define TINY_PROFILER_TIMER_FUNCTION_OUT(deltaTime)

#define TINY_PROFILER_LOG_TIME()
#endif // TINY_DEBUG

