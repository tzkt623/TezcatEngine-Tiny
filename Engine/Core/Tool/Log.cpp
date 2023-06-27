#include "Log.h"

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
			break;
		case LogTypeID::LT_Info:
			data->str.insert(0, "[Info]");
			break;
		case LogTypeID::LT_Error:
			data->str.insert(0, "[Error]");
			break;
		case LogTypeID::LT_Warning:
			data->str.insert(0, "[Warning]");
			break;
		default:
			break;
		}

		return data;
	}

	void Log::info(std::string&& info)
	{
		auto result = createString(LogTypeID::LT_Info, std::forward<std::string>(info));
		push(result);
	}

	void Log::error(std::string&& info)
	{
		auto result = createString(LogTypeID::LT_Error, std::forward<std::string>(info));
		push(result);
	}

	void Log::warning(std::string&& info)
	{
		auto result = createString(LogTypeID::LT_Warning, std::forward<std::string>(info));
		push(result);
	}

	void Log::engine(std::string&& info)
	{
		auto result = createString(LogTypeID::LT_Engine, std::forward<std::string>(info));
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
