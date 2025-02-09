#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	typedef unsigned char LogType;

	enum LogTypeID : LogType
	{
		LT_Engine = 0,
		LT_Info,
		LT_Error,
		LT_Warning,
	};

	struct LogData
	{
		LogTypeID type;
		std::string str;
	};

	class TINY_API LogImp
	{
	public:
		LogImp() = default;
		virtual ~LogImp() = default;
	};

	class TINY_API Log
	{
	public:
		Log() = delete;
		~Log() = delete;

	public:
		static void engine(std::string&& info);
		static void info(std::string&& info);

		static void push(LogData* result);

		static void error(std::string&& info);
		static void warning(std::string&& info);

		static void clear();

		static void setImp(LogImp* imp)
		{
			delete mLogImp;
			mLogImp = imp;
		}

		static std::deque<LogData*>& allLog() { return mLogQueue; }

	private:
		static LogData* createString(const LogTypeID& type, std::string&& info);

	private:
		static LogImp* mLogImp;
		static std::deque<LogData*> mLogQueue;
		static std::stack<LogData*> mFreeStrings;
	};


	class TINY_API DefaultLogImp : public LogImp
	{
	public:

	private:

	};


#ifndef TINY_Release
#define Log_Engine(x) Log::engine(x);
#define Log_Info(x) Log::info(x);
#define Log_Warning(x) Log::warning(x);
#define Log_Error(x) Log::error(x);
#else
#define Log_Engine(x)
#define Log_Info(x)
#define Log_Warning(x)
#define Log_Error(x)
#endif
}
