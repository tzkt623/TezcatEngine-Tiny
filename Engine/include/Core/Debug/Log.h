#pragma once
/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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
		static bool isDirty() { return mDirty; }
		static void clearDirty() { mDirty = false; }

	private:
		static LogData* createString(const LogTypeID& type, std::string&& info);

	private:
		static LogImp* mLogImp;
		static std::deque<LogData*> mLogQueue;
		static std::stack<LogData*> mFreeStrings;
		static bool mDirty;
	};


	class TINY_API DefaultLogImp : public LogImp
	{
	public:

		void debug()
		{
			auto s = std::format("{}", 1);
		}

	private:

	};

#define TINY_LOG_COLOR_RESET	"\033[0m"
#define TINY_LOG_COLOR_BLACK	"\033[30m" /* Black */
#define TINY_LOG_COLOR_RED		"\033[31m" /* Red */
#define TINY_LOG_COLOR_GREEN	"\033[32m" /* Green */
#define TINY_LOG_COLOR_YELLOW	"\033[33m" /* Yellow */
#define TINY_LOG_COLOR_BLUE		"\033[34m" /* Blue */
#define TINY_LOG_COLOR_PURPLE	"\033[35m" /* Purple */
#define TINY_LOG_COLOR_CYAN		"\033[36m" /* Cyan */
#define TINY_LOG_COLOR_WHITE	"\033[37m" /* White */

#ifndef TINY_RELEASE
#define TINY_LOG_ENGINE(x) Log::engine(x);
#define TINY_LOG_INFO(x) Log::info(x);
#define TINY_LOG_WARNING(x) Log::warning(x);
#define TINY_LOG_ERROR(x) Log::error(x);
#define TINY_LOG_CONSOLO(color, x) std::cerr << color << x << "\n";
#else
#define TINY_LOG_ENGINE(x)
#define TINY_LOG_INFO(x)
#define TINY_LOG_WARNING(x)
#define TINY_LOG_ERROR(x)
#define TINY_LOG_CONSOLO(color, x)
#endif
}
