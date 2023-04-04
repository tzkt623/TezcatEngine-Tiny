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
			if (sStringBuffer.capacity() < size)
			{
				sStringBuffer.reserve(size);
			}
			std::snprintf(sStringBuffer.data(), size, str, args...);
			return std::string(sStringBuffer.data(), size);
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
		static std::vector<char> sStringBuffer;
	};
}

