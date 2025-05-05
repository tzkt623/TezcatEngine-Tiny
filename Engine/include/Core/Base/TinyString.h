#pragma once
/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../Head/ConfigHead.h"
#include <string>
#include <queue>

namespace tezcat::Tiny
{
	class TINY_API TinyString
	{
	public:
		TinyString();
		TinyString(const char* str);
		TinyString(const std::string& str);
		TinyString(std::string&& str);
		~TinyString();

		operator std::string&() const { return *mString; }

		std::string& get() const { return *mString; }

		TinyString& operator=(std::string&& str)
		{
			*mString = std::move(str);
			return *this;
		}

		TinyString& operator=(const std::string& str)
		{
			*mString = str;
			return *this;
		}

	private:
		std::string* mString;

	private:
		static std::string* create();
		static std::queue<std::string*> Pool;
	};
}
