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

#include "Core/Debug/Profiler.h"

namespace tezcat::Tiny
{
	//------------------------------------------------------
	//
	//	Profiler
	//
	int Profiler::PassCount = 0;
	int Profiler::DrawCall = 0;
	float2 Profiler::mouseOffset(0.0f);
	float2 Profiler::mousePosition;


	size_t Profiler::MemoryAlloc;
	size_t Profiler::MemoryFree;

	double Profiler::RenderTime;
	double Profiler::LogicTime;

	const unsigned char* Profiler::GPU;



	//------------------------------------------------------
	//
	//	ProfilerTimer
	//
	double ProfilerTimer::DefaultDeltaTime;
	const char* NoName = "##NoTimerName##";

	ProfilerTimer::ProfilerTimer()
		: ProfilerTimer(NoName, ProfilerTimer::DefaultDeltaTime)
	{

	}

	ProfilerTimer::ProfilerTimer(double& deltaTime)
		: ProfilerTimer(NoName, deltaTime)
	{
		mStartTime = std::chrono::high_resolution_clock::now();
	}

	ProfilerTimer::ProfilerTimer(const char* functionName)
		: ProfilerTimer(functionName, ProfilerTimer::DefaultDeltaTime)
	{

	}

	ProfilerTimer::ProfilerTimer(const char* functionName, double& deltaTime)
		: mName(functionName)
		, mDeltaTime(deltaTime)
		, mNoStop(true)
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
		if (mNoStop)
		{
			mDeltaTime = this->stopOut();
		}
	}

	double ProfilerTimer::stopOut()
	{
		mNoStop = false;

		auto end_time = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartTime)
			.time_since_epoch()
			.count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time)
			.time_since_epoch()
			.count();

		auto duration = end - start;
		return duration * 0.001;
	}
}


