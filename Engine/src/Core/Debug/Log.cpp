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

#include "Core/Debug/Log.h"

namespace tezcat::Tiny
{
	LogImp* Log::mLogImp = new DefaultLogImp();
	std::deque<LogData*> Log::mLogQueue;
	std::stack<LogData*> Log::mFreeStrings;

	LogData* Log::createString(const LogTypeID& type, std::string&& info)
	{
		LogData* data = nullptr;

		if (mFreeStrings.empty())
		{
			data = new LogData();
			data->str = std::move(info);
			data->type = type;
		}
		else
		{
			data = mFreeStrings.top();
			data->str = std::move(info);
			data->type = type;
			mFreeStrings.pop();
		}

		switch (data->type)
		{
		case LogTypeID::LT_Engine:
			data->str.insert(0, "[Engine]");
			TINY_LOG_CONSOLO(TINY_LOG_COLOR_GREEN, data->str);
			break;
		case LogTypeID::LT_Info:
			data->str.insert(0, "[Info]");
			TINY_LOG_CONSOLO(TINY_LOG_COLOR_WHITE, data->str);
			break;
		case LogTypeID::LT_Error:
			data->str.insert(0, "[Error]");
			TINY_LOG_CONSOLO(TINY_LOG_COLOR_RED, data->str);
			break;
		case LogTypeID::LT_Warning:
			data->str.insert(0, "[Warning]");
			TINY_LOG_CONSOLO(TINY_LOG_COLOR_YELLOW, data->str);
			break;
		default:
			break;
		}


		return data;
	}

	void Log::info(std::string&& info)
	{
		auto result = createString(LogTypeID::LT_Info, TINY_FWD(info));
		push(result);
	}

	void Log::error(std::string&& info)
	{
		auto result = createString(LogTypeID::LT_Error, TINY_FWD(info));
		push(result);
	}

	void Log::warning(std::string&& info)
	{
		auto result = createString(LogTypeID::LT_Warning, TINY_FWD(info));
		push(result);
	}

	void Log::engine(std::string&& info)
	{
		auto result = createString(LogTypeID::LT_Engine, TINY_FWD(info));
		push(result);
	}

	void Log::push(LogData* result)
	{
		mLogQueue.push_back(result);
		if (mLogQueue.size() > 500)
		{
			mFreeStrings.push(mLogQueue.front());
			mLogQueue.pop_front();
		}
	}

	void Log::clear()
	{
		for (auto s : mLogQueue)
		{
			mFreeStrings.push(s);
		}

		mLogQueue.clear();
	}
}
