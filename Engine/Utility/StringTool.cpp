#include "StringTool.h"

namespace tezcat::Tiny::Utility
{
	std::vector<char> StringTool::s_StringBuffer(4);

	std::string StringTool::fromU8(const std::u8string& string)
	{
		return std::string(string.begin(), string.end());
	}
}