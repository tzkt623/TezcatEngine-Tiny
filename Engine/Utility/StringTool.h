#pragma once
#include "Core/Head/ConfigHead.h"
#include "Core/Head/CppHead.h"

namespace tezcat::Tiny::Utility
{
	class TINY_API StringTool
	{
	public:
		static std::string fromU8(const std::u8string& string);

		template<typename... Args>
		static std::string stringFormat(const std::string& format, Args... args)
		{
			auto str = format.c_str();
			auto size = 1 + std::snprintf(nullptr, 0, str, args...);
			if (s_StringBuffer.capacity() < size)
			{
				s_StringBuffer.reserve(size);
			}
			std::snprintf(s_StringBuffer.data(), size, str, args...);
			return std::string(s_StringBuffer.data(), size);
		}

		template<typename... Args>
		static int stringFormat(std::string& container, const std::string& format, Args... args)
		{
			auto str = format.c_str();
			auto size = 1 + std::snprintf(nullptr, 0, str, args...);
			container.reserve(size);
			std::snprintf(container.data(), size, str, args...);
			return size;
		}

	private:
		static std::vector<char> s_StringBuffer;
	};
}

