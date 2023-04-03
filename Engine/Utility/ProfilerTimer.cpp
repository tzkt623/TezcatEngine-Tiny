#include "ProfilerTimer.h"

namespace tezcat::Tiny::Utility
{
	double ProfilerTimer::DefaultDeltaTime;
	const char* NoName = "##NoTimerName##";

	ProfilerTimer::ProfilerTimer()
		: ProfilerTimer(NoName, ProfilerTimer::DefaultDeltaTime)
	{

	}

	ProfilerTimer::ProfilerTimer(double& deltaTime)
		: ProfilerTimer(NoName, deltaTime)
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	ProfilerTimer::ProfilerTimer(const char* functionName)
		: ProfilerTimer(functionName, ProfilerTimer::DefaultDeltaTime)
	{

	}

	ProfilerTimer::ProfilerTimer(const char* functionName, double& deltaTime)
		: mName(functionName)
		, m_DeltaTime(deltaTime)
		, m_NoStop(true)
	{

	}



	ProfilerTimer::~ProfilerTimer()
	{
		this->stop();
		mName = nullptr;
		//		std::cout  << duration << "us (" << ms << "ms)" << std::endl;
	}

	void ProfilerTimer::stop()
	{
		if (m_NoStop)
		{
			m_DeltaTime = this->stopOut();
		}
	}

	double ProfilerTimer::stopOut()
	{
		m_NoStop = false;

		auto end_time = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime)
			.time_since_epoch()
			.count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time)
			.time_since_epoch()
			.count();

		auto duration = end - start;
		return duration * 0.001;
	}
}