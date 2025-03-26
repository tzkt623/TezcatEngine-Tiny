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

#include "Core/Renderer/RenderObjectCache.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/RenderObserver.h"
#include "Core/Renderer/Renderer.h"

#include "Core/Event/EngineEvent.h"


namespace tezcat::Tiny
{
	std::array<RenderObjectCache*, 32> RenderObjectCache::sLayerAry;

	RenderObjectCache::RenderObjectCache()
		: mIndex(-1)
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_BeforeSceneExit
			, this
			, [this](const EventData& data)
			{
				mRenderObjectList.clear();
			});
	}

	RenderObjectCache::~RenderObjectCache()
	{
		mRenderObjectList.clear();
	}

	void RenderObjectCache::init()
	{
		for (auto i = 0; i < sLayerAry.size(); i++)
		{
			sLayerAry[i] = new RenderObjectCache();
		}
	}

	void RenderObjectCache::addRenderAgent(uint32_t index, BaseRenderer* renderAgent)
	{
		sLayerAry[index]->mRenderObjectList.push_back(renderAgent);
	}

	void RenderObjectCache::culling(int layerIndex, ReplacedPipelinePass* pass)
	{
		auto& render_object_list = sLayerAry[layerIndex]->mRenderObjectList;
		if (render_object_list.empty())
		{
			return;
		}

		auto it = render_object_list.begin();
		auto end = render_object_list.end();
		while (it != end)
		{
			if (auto renderer = it->lock())
			{
				renderer->makeRenderCommand(pass);
				it++;
			}
			else
			{
				it = render_object_list.erase(it);
				end = render_object_list.end();
			}
		}
	}

	void RenderObjectCache::culling(int layerIndex, BaseRenderObserver* renderObserver)
	{
		auto& render_object_list = sLayerAry[layerIndex]->mRenderObjectList;
		if (render_object_list.empty())
		{
			return;
		}

		auto it = render_object_list.begin();
		auto end = render_object_list.end();
		while (it != end)
		{
			if (auto renderer = it->lock())
			{
				renderer->makeRenderCommand(renderObserver);
				it++;
			}
			else
			{
				it = render_object_list.erase(it);
				end = render_object_list.end();
			}
		}
	}

}
