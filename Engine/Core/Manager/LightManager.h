#pragma once

#include "../Head/CppHead.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	class DirectionalLight;
	class PointLight;
	class Spotlight;

	class LightData
	{
	public:
		LightData();
		~LightData();

	public:
		DirectionalLight* directionalLight;
		std::vector<PointLight*> pointLights;
		std::vector<Spotlight*> spotLights;
	};


	class LightManager
	{
	public:
		LightManager();
		~LightManager();

	public:
		void setData(LightData* data)
		{
			m_Data = data;
		}

		LightData* getData()
		{
			return m_Data;
		}

	private:
		LightData* m_Data;
	};
}