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

#include "Core/LightLayer.h"
#include "Core/Component/GameObject.h"
#include "Core/Component/Light.h"

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
