#pragma once

#include <string>
#include <vector>
#include <stack>
#include <cstdio>
#include <list>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <regex>
#include <typeindex>
#include <type_traits>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <array>
#include <string_view>
#include <tuple>
#include <limits>
#include <ranges>
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

#define TinyMove(x) std::move(x)

#ifndef TINY_Release
#define TinyThrow(x) throw x
#define TinyThrow_Logic(x) throw std::logic_error(x)
#define TinyThrow_Runtime(condition, message) if(condition) { fmt::print("=======RuntimeError=======\nFile:{1}\nLine:{2}\n{0}\n==========================\n", message, __FILE__, __LINE__); abort(); }
#define TinyAssert(x) assert(x)
#else
#define TinyThrow(x)
#define TinyThrow_Logic(x)
#define TinyThrow_Runtime(condition, info)
#define TinyAssert(x)
#endif
	}
}
