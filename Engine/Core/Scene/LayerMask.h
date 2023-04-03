#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class TINY_API LayerMask
	{
	public:
		static void init();

		static const std::string& getName(int index);
		static void setMask(int index, const std::string& maskName);

		static uint32_t getMask(const std::string& maskName);
		static uint32_t getMask(uint32_t index);

	private:
		static std::vector<std::string> sMaskWithName;
	};
}
