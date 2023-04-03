#include "LightLayer.h"
#include "../Component/GameObject.h"
#include "../Component/Light.h"

namespace tezcat::Tiny::Core
{
	LightLayer::LightLayer()
	{

	}

	LightLayer::~LightLayer()
	{

	}

	void LightLayer::addLight(PointLight* light)
	{
		m_LightList.emplace_back(light);
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

	bool LightLayer::testGameObject(Pipeline* pipeline, IRenderObejct* renderObject)
	{
		if (m_LightList.empty())
		{
			return false;
		}

		//#RenderLayerCull
		auto it = m_LightList.begin();
		auto end = m_LightList.end();
		while (it != end)
		{
			auto com = dynamic_cast<Component*>(*it);
			if (com->isEnable())
			{
				if ((*it)->cullGameObject(dynamic_cast<Component*>(renderObject)->getGameObject()))
				{
//					pipeline->addPassLightingTestRenderObject(*it, renderObject);
				}
				it++;
			}
			else
			{
				it = m_LightList.erase(it);
				end = m_LightList.end();
			}
		}

		return false;
	}
}
