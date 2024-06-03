#include "LightLayer.h"
#include "../Component/GameObject.h"
#include "../Component/Light.h"

namespace tezcat::Tiny
{
	LightLayer::LightLayer()
	{

	}

	LightLayer::~LightLayer()
	{

	}

	void LightLayer::addLight(PointLight* light)
	{
		//mLightList.emplace_back(light);
	}

	void LightLayer::addLight(SpotLight* light)
	{

	}

	void LightLayer::addLight(ILight* light)
	{
		switch (light->getLightType())
		{
		case LightType::Directional:
			break;
		case LightType::Point:
			this->addLight(dynamic_cast<PointLight*>(light));
			break;
		case LightType::Spot:
			this->addLight(dynamic_cast<SpotLight*>(light));
			break;
		default:
			break;
		}
	}

	bool LightLayer::testGameObject(Pipeline* pipeline, BaseRenderer* renderObject)
	{
		if (mLightList.empty())
		{
			return false;
		}

		//#RenderLayerCull
		auto it = mLightList.begin();
		auto end = mLightList.end();
		while (it != end)
		{
			auto com = dynamic_cast<Component*>(*it);
			if (com->isEnable())
			{
// 				if ((*it)->culling(com->getGameObject()))
// 				{
//					pipeline->addPassLightingTestRenderObject(*it, renderObject);
//				}
				it++;
			}
			else
			{
				it = mLightList.erase(it);
				end = mLightList.end();
			}
		}

		return false;
	}
}
