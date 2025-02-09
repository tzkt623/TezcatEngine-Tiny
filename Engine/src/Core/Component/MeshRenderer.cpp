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

#include "Core/Component/MeshRenderer.h"
#include "Core/Component/Transform.h"
#include "Core/Component/GameObject.h"

#include "Core/Renderer/RenderObjectCache.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(MeshRenderer, ComponentT<MeshRenderer>)

	MeshRenderer::MeshRenderer()
		: mRenderAgent(nullptr)
	{
		mRenderAgent = MeshRenderAgent::create();
		mRenderAgent->saveObject();
	}

	MeshRenderer::~MeshRenderer()
	{
		mRenderAgent->deleteObject();
	}

	void MeshRenderer::onEnable()
	{
		if (auto transform = mGameObject->getComponent<Transform>())
		{
			mRenderAgent->setTransform(transform);
		}
	}

	void MeshRenderer::onDisable()
	{

	}

	void MeshRenderer::onStart()
	{
		RenderObjectCache::addRenderAgent(this->getGameObject()->getLayerIndex(), mRenderAgent);
	}

	void MeshRenderer::onComponentAdded(Component* component)
	{
		if (component->typeIDEqual<Transform>())
		{
			mRenderAgent->setTransform((Transform*)component);
		}
	}

	void MeshRenderer::onComponentRemoved(Component* component)
	{

	}

}
