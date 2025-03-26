#pragma once
/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

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
#include <cstdio>
#include <cstdint>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <memory>

#include <regex>
#include <string>
#include <string_view>
#include <format>

#include <vector>
#include <stack>
#include <queue>
#include <array>
#include <list>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <variant>

#include <typeindex>
#include <type_traits>

#include <chrono>
#include <random>
#include <tuple>
#include <limits>
#include <ranges>
#include <functional>
#include <filesystem>

#include <mutex>
#include <thread>
#include <semaphore>
#include <condition_variable>

#include "ConfigHead.h"

#define FMT_HEADER_ONLY
#include "ThirdParty/fmt/format.h"
#include "ThirdParty/fmt/printf.h"


namespace tezcat::Tiny
{
	//using int8 = int8_t;
	//using int16 = int16_t;
	//using int32 = int32_t;
	//using int64 = int64_t;
	//
	//using uint8 = uint8_t;
	//using uint16 = uint16_t;
	//using uint32 = uint32_t;
	//using uint64 = uint64_t;

	inline namespace v1
	{
		template<typename T, typename... Args>
		constexpr std::shared_ptr<T> TinyShared(Args&& ... args)
		{
			return std::make_shared<T>(std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		constexpr std::unique_ptr<T> TinyUnique(Args&& ... args)
		{
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

		template<typename T1, typename T2>
		constexpr std::shared_ptr<T1> TinyStaticCast(const T2& pointer)
		{
			return std::static_pointer_cast<T1>(pointer);
		}

		template<typename T1, typename T2>
		constexpr std::shared_ptr<T1> TinyDynamicCast(const T2& pointer)
		{
			return std::dynamic_pointer_cast<T1>(pointer);
		}
	}

#define TINY_BIND_THIS(func) std::bind_front(&func, this)
#define TINY_BIND_THIS0(func) std::bind(&func, this)
#define TINY_BIND_THIS1(func) std::bind(&func, this, std::placeholders::_1)
#define TINY_BIND_THIS2(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2)
#define TINY_BIND_THIS3(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define TINY_BIND_THIS4(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

#define TINY_BIND(func) std::bind_front(&func)
#define TINY_BIND0(func) std::bind(&func)
#define TINY_BIND1(func) std::bind(&func, std::placeholders::_1)
#define TINY_BIND2(func) std::bind(&func, std::placeholders::_1, std::placeholders::_2)
#define TINY_BIND3(func) std::bind(&func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define TINY_BIND4(func) std::bind(&func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

#define TINY_FWD(x) std::forward<decltype(x)>(x)

#ifndef TINY_RELEASE
#define TINY_THROW(x) throw x
#define TINY_THROW_LOGIC(x) throw std::logic_error(x)
	//#define TINY_THROW_RUNTIME(condition, message) if(condition) { fmt::print("=======RuntimeError=======\nFile:{1}\nLine:{2}\n{0}\n==========================\n", message, __FILE__, __LINE__); __debugbreak(); }
#define TINY_THROW_RUNTIME(condition, message) if(condition) { throw std::runtime_error(std::format("=======RuntimeError=======\nFile:{1}\nLine:{2}\n{0}\n==========================\n", message, __FILE__, __LINE__)); }
#define TINY_ASSERT(x) assert(x);
#define TINY_BREAK(x) if(!(x)) { __debugbreak(); }
#else
#define TINY_THROW(x)
#define TINY_THROW_LOGIC(x)
#define TINY_THROW_RUNTIME(condition, info)
#define TINY_ASSERT(x)
#endif

	template<class>
	struct FileStringSelector
	{
	};

	template<>
	struct FileStringSelector<std::string>
	{
		using string_view = std::string_view;
	};

	template<>
	struct FileStringSelector<std::wstring>
	{
		using string_view = std::wstring_view;
	};

	namespace file_sys = std::filesystem;
	using file_path = std::filesystem::path;
	using file_string = file_path::string_type;
	using file_string_view = FileStringSelector<file_string>::string_view;


	struct file_sys_helper
	{
		static auto generic(const file_path& path)
		{
			if constexpr (std::is_same_v<file_string, std::string>)
			{
				return path.generic_string();
			}
			else if constexpr (std::is_same_v<file_string, std::wstring>)
			{
				return path.generic_wstring();
			}
		}

		static auto string(const file_path& path)
		{
			if constexpr (std::is_same_v<file_string, std::string>)
			{
				return path.string();
			}
			else if constexpr (std::is_same_v<file_string, std::wstring>)
			{
				return path.wstring();
			}
		}

		template<class... Args>
		std::wstring format(std::wformat_string<Args...> fmt, Args&&... args)
		{
			if constexpr (std::is_same_v<Args, std::wstring>)
			{
				return std::vformat(fmt.get(), std::make_wformat_args(args...));
			}
		}

		template<class Arg>
		auto to_format(const char* fmt)
		{
			if (std::is_same_v<Arg, std::wstring>)
			{
				return std::wstring(fmt);
			}
		}

	};

#ifdef _WIN32
#define TINY_FILE_FORMAT(fmt,...) std::format(L#fmt, ##__VA_ARGS__);
#else
#define TINY_FILE_FORMAT(fmt,...) std::format(#fmt, ##__VA_ARGS__);
#endif
}
