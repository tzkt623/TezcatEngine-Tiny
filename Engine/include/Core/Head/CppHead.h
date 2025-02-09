#pragma once

#include <cstdio>
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
	using int8 = int8_t;
	using int16 = int16_t;
	using int32 = int32_t;
	using int64 = int64_t;

	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;

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

#define TINY_BIND_THIS0(func) std::bind(&func, this)
#define TINY_BIND_THIS1(func) std::bind(&func, this, std::placeholders::_1)
#define TINY_BIND_THIS2(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2)
#define TINY_BIND_THIS3(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define TINY_BIND_THIS4(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

#define TINY_BIND0(func) std::bind(&func)
#define TINY_BIND1(func) std::bind(&func, std::placeholders::_1)
#define TINY_BIND2(func) std::bind(&func, std::placeholders::_1, std::placeholders::_2)
#define TINY_BIND3(func) std::bind(&func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define TINY_BIND4(func) std::bind(&func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

#define TINY_FWD(x) std::forward<decltype(x)>(x)

#ifndef TINY_RELEASE
#define TINY_THROW(x) throw x
#define TINY_THROW_LOGIC(x) throw std::logic_error(x)
#define TINY_THROW_RUNTIME(condition, message) if(condition) { fmt::print("=======RuntimeError=======\nFile:{1}\nLine:{2}\n{0}\n==========================\n", message, __FILE__, __LINE__); abort(); }
#define TINY_ASSERT(x) assert(x)
#else
#define TINY_THROW(x)
#define TINY_THROW_LOGIC(x)
#define TINY_THROW_RUNTIME(condition, info)
#define TINY_ASSERT(x)
#endif
}
