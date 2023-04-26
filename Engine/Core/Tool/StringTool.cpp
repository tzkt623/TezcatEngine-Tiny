#include "StringTool.h"

namespace tezcat::Tiny
{
	std::vector<char> StringTool::sStringBuffer(4);

	std::string StringTool::fromU8(const std::u8string& string)
	{
		return std::string(string.begin(), string.end());
	}
}
