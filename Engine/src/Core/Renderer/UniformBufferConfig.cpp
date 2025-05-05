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

#include "Core/Renderer/UniformBufferConfig.h"

namespace tezcat::Tiny
{

	void UniformBufferLayout::pushLayout(const std::string& name, const int32_t& offset, const uint32_t& size)
	{
		auto slot = this->findSlot(name);
		if (slot)
		{
			slot->offset = offset;
			slot->size = size;
		}
		else
		{
			mSlot.emplace_back(name, offset, size);
		}
	}

	UniformBufferLayout::Slot* UniformBufferLayout::findSlot(const std::string_view& name)
	{
		for (auto& slot : mSlot)
		{
			if (slot.name == name)
			{
				return &slot;
			}
		}

		return nullptr;
	}

	bool UniformBufferLayout::isValied()
	{
		for (auto& slot : mSlot)
		{
			if (slot.offset == -1)
			{
				return false;
			}
		}

		return true;
	}

}
