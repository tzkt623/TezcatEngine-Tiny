#include "RenderLayer.h"

#include "BaseGraphics.h"
#include "RenderObject.h"
#include "RenderPass.h"

#include "../Component/Component.h"
#include "../Component/Camera.h"
#include "../Component/Light.h"

#include "../Manager/LightManager.h"
#include "../Manager/ShaderManager.h"

#include "../Pipeline/Pipeline.h"

#include "../Event/EngineEvent.h"


namespace tezcat::Tiny
{
	std::array<RenderLayer*, 32> RenderLayer::sLayerAry;

	RenderLayer::RenderLayer()
		: mIndex(-1)
	{
		EngineEvent::get()->addListener(EngineEventID::EE_PopScene, this
			, [this](const EventData& data)
			{
				mRenderObjectList.clear();
			});
	}

	RenderLayer::~RenderLayer()
	{
		mRenderObjectList.clear();
	}

	void RenderLayer::culling(IRenderObserver* renderObserver, const RenderPassType& passType)
	{
		if (mRenderObjectList.empty())
		{
			return;
		}

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
						agent->getRenderObject()->sendToRenderPass(passType);
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
		mRenderObjectList.emplace_back(renderAgent);
	}

	void RenderLayer::init()
	{
		for (auto i = 0; i < sLayerAry.size(); i++)
		{
			sLayerAry[i] = new RenderLayer();
		}
	}

}
