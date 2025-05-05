#include "Core/Base/TinyString.h"

namespace tezcat::Tiny
{
	std::queue<std::string*> TinyString::Pool;

	TinyString::TinyString()
		: mString(create())
	{

	}

	TinyString::TinyString(const char* str)
		: mString(create())
	{
		*mString = str;
	}

	TinyString::TinyString(const std::string& str)
		: mString(create())
	{
		*mString = str;
	}

	TinyString::TinyString(std::string&& str)
		: mString(create())
	{
		*mString = std::move(str);
	}

	TinyString::~TinyString()
	{
		mString->clear();
		Pool.push(mString);
	}

	std::string* TinyString::create()
	{
		if (Pool.empty())
		{
			return new std::string();
		}
		else
		{
			auto str = Pool.front();
			Pool.pop();
			return str;
		}
	}
}

