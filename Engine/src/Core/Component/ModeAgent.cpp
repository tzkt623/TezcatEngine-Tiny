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


#include "Core/Component/GameObject.h"
#include "Core/Component/ModeAgent.h"
#include "Core/Component/Transform.h"
#include "Core/Component/MeshRenderer.h"

#include "Core/Manager/ResourceManager.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(ModeAgent, ComponentAutoID<ModeAgent>);

	ModeAgent::~ModeAgent()
	{

	}

	void ModeAgent::loadMode(const std::string_view& view)
	{
		Model* mode = ResourceManager::loadAndSave<Model>(view.data());
	}

	void ModeAgent::addSubMesh(MeshRenderer* meshRenderer)
	{
		GameObject* go = GameObject::create();
		auto transform = go->addComponent<Transform>();

		auto renderer = go->addComponent<MeshRenderer>();
	}
}
