﻿#pragma once
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
#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"

namespace tezcat::Tiny
{
	template<typename CreatorType, typename BaseType>
	class TINY_API DelegateCreator
	{
	public:
		static void attach(CreatorType* creator)
		{
			sCreator = creator;
		}

		static void destroy()
		{
			delete sCreator;
		}

		template<typename... Args>
		static BaseType* create(Args&&... args)
		{
			return sCreator->create(std::forward<Args>(args)...);
		}

	private:
		static CreatorType* sCreator;
	};

	template<typename CreatorType, typename BaseType>
	CreatorType* DelegateCreator<CreatorType, BaseType>::sCreator = nullptr;
}
