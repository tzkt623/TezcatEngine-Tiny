#include "RenderLayer.h"

#include "BaseGraphics.h"
#include "RenderObject.h"
#include "RenderPass.h"

#include "../Component/Component.h"
#include "../Component/Camera.h"
#include "../Component/Light.h"

#include "../Manager/LightManager.h"
#include "../Manager/ShaderManager.h"

#include "../Event/EngineEvent.h"


namespace tezcat::Tiny
{
	std::array<RenderLayer*, 32> RenderLayer::sLayerAry;

	RenderLayer::RenderLayer()
		: mIndex(-1)
	{
		EngineEvent::get()->addListener(EngineEventID::EE_OnPopScene
			, this
			, [this](const EventData& data)
			{
				mRenderObjectList.clear();
			});
	}

	RenderLayer::~RenderLayer()
	{
		mRenderObjectList.clear();
	}

	void RenderLayer::culling(BaseGraphics* graphics, IRenderObserver* renderObserver, RenderQueue* queue)
	{
		if (mRenderObjectList.empty())
		{
			return;
		}

		auto phase = renderObserver->getPipelinePhase();
		auto it = mRenderObjectList.begin();
		auto end = mRenderObjectList.end();
		while (it != end)
		{
			auto agent = (*it);
			if (agent.lock())
			{
				auto com = agent->getComponent();
				if (com->isEnable())
				{
					if (renderObserver->culling(com->getGameObject()))
					{
						agent->sendToQueue(graphics, phase, queue);
					}
				}

				it++;
			}
			else
			{
				it = mRenderObjectList.erase(it);
			}
		}
	}

	void RenderLayer::addRenderAgent(RenderAgent* renderAgent)
	{
		mRenderObjectList.push_back(renderAgent);
	}

	void RenderLayer::init()
	{
		for (auto i = 0; i < sLayerAry.size(); i++)
		{
			sLayerAry[i] = new RenderLayer();
		}
	}
}
