#include "RenderObjectCache.h"
#include "BaseGraphics.h"
#include "RenderObserver.h"
#include "Renderer.h"

#include "../Event/EngineEvent.h"


namespace tezcat::Tiny
{
	std::array<RenderObjectCache*, 32> RenderObjectCache::sLayerAry;

	RenderObjectCache::RenderObjectCache()
		: mIndex(-1)
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_OnPopScene
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

	void RenderObjectCache::addRenderAgent(uint32 index, BaseRenderer* renderAgent)
	{
		sLayerAry[index]->mRenderObjectList.push_back(renderAgent);
	}

	void RenderObjectCache::culling(int layerIndex,  PipelinePass* pass)
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
			if (auto renderer = (*it).lock())
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
			if (auto renderer = (*it).lock())
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
