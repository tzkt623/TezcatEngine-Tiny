#include "RenderLayer.h"
#include "LightLayer.h"

#include "../Renderer/RenderObject.h"
#include "../Component/Component.h"
#include "../Component/Camera.h"
#include "../Manager/LightManager.h"
#include "../Pipeline/Pipeline.h"
#include "../Component/Light.h"
#include "../Renderer/BaseGraphics.h"
#include "../Manager/ShaderManager.h"
#include "../Renderer/RenderPass.h"

namespace tezcat::Tiny::Core
{
	std::array<RenderLayer*, 32> RenderLayer::sLayerAry =
	{
		new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),
		new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),
		new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),
		new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer(),new RenderLayer()
	};

	RenderLayer::RenderLayer()
		: mIndex(-1)
	{

	}

	RenderLayer::~RenderLayer()
	{
		mRenderObjectList.clear();
	}

	void RenderLayer::addRenderObejct(IRenderObject* renderObject)
	{
		mRenderObjectList.emplace_back(renderObject);
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
			auto com = dynamic_cast<Component*>(*it);
			if (com->isEnable())
			{
				if (renderObserver->culling(com->getGameObject()))
				{
					(*it)->sendToRenderPass(passType);
				}
			}
			it++;
		}
	}
}
