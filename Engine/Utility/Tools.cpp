#include "Tools.h"

namespace tezcat::Tiny::Utility
{
	std::vector<char> Tools::s_StringBuffer(4);

	std::string Tools::fromU8(const std::u8string& string)
	{
		return std::string(string.begin(), string.end());
	}


	//---------------------------------------------
	// 
	//  [1/27/2022 Administrator]
	//

}
