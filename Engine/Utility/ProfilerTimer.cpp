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
		: m_Name(functionName)
		, m_DeltaTime(deltaTime)
	{

	}



	ProfilerTimer::~ProfilerTimer()
	{
		this->stop();
		m_Name = nullptr;
		//		std::cout  << duration << "us (" << ms << "ms)" << std::endl;
	}

	void ProfilerTimer::stop()
	{
		this->stop(m_DeltaTime);
	}

	void ProfilerTimer::stop(double& time)
	{
		auto end_time = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime)
			.time_since_epoch()
			.count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time)
			.time_since_epoch()
			.count();

		auto duration = end - start;
		time = duration * 0.001;
	}
}