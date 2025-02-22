#pragma once
/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

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
#include "../Base/TinyObject.h"
#include "Component.h"

namespace tezcat::Tiny
{
	class MeshRenderer;
	class TINY_API ModeAgent : public ComponentT<ModeAgent>
	{
		ModeAgent();
		TINY_OBJECT_H(ModeAgent, ComponentT<ModeAgent>)

	public:
		virtual ~ModeAgent();


	public:
		void loadMode(const std::string_view& view);

		void addSubMesh(MeshRenderer* mesh);

	private:
		std::vector<MeshRenderer*> mSubMeshArray;
	};
}



