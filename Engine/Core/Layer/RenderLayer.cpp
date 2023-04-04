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

	void RenderLayer::testWithCamera(Camera* camera)
	{
		if (mRenderObjectList.empty())
		{
			return;
		}

		//#RenderLayerCulling
		auto it = mRenderObjectList.begin();
		auto end = mRenderObjectList.end();
		while (it != end)
		{
			auto com = (Component*)(*it);
			if (com->isEnable())
			{
				if (camera->frustumCulling(com->getGameObject()))
				{
					(*it)->sendToRenderPass();
				}
			}
			it++;
		}
	}

	void RenderLayer::testVisibleObjects(ILight* light)
	{
// 		auto pass = new PassShader(light->getShader(), light);
// 		for (auto object : mVisibleRenderObjectList)
// 		{
// 			pass->addMeshRenderer(object);
// 		}
	}
}
