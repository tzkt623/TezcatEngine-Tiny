#pragma once

/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "TinyRefObject.h"

namespace tezcat::Tiny
{
	class TINY_API TinyObject : public TinyRefObject
	{
		TINY_ABSTRACT_OBJECT_H(TinyObject, TinyRefObject)
	public:
		TinyObject() {}
		virtual ~TinyObject() = 0;

	public:
		static void destroy(TinyObject* obj);
	};
}
