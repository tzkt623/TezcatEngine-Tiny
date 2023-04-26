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
#include "ConfigHead.h"

namespace tezcat::Tiny
{
	inline namespace v1
	{
		template<typename T>
		using TinyRef = std::shared_ptr<T>;

		template<typename T>
		using TinyURef = std::unique_ptr<T>;

		template<typename T>
		using TinyWRef = std::weak_ptr<T>;

		template<typename T, typename... Args>
		constexpr TinyRef<T> TinyShared(Args&& ... args)
		{
			return std::make_shared<T>(std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		constexpr TinyURef<T> TinyUnique(Args&& ... args)
		{
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

		template<typename T1, typename T2>
		constexpr TinyRef<T1> TinyStaticCast(const T2& pointer)
		{
			return std::static_pointer_cast<T1>(pointer);
		}

		template<typename T1, typename T2>
		constexpr TinyRef<T1> TinyDynamicCast(const T2& pointer)
		{
			return std::dynamic_pointer_cast<T1>(pointer);
		}

#define TinyMove(x) std::move(x)
	}
}
