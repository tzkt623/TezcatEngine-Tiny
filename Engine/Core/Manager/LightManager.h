#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "Utility/Utility.h"

namespace tezcat::Tiny::Core
{
	class DirectionalLight;
	class PointLight;
	class SpotLight;


	class TINY_API LightData
	{
	public:
		LightData();
		~LightData();

	public:
		DirectionalLight* directionalLight;
		std::vector<PointLight*> pointLights;
		std::vector<SpotLight*> spotLights;
	};


	class TINY_API LightManager
	{
	public:
		LightManager();
		~LightManager();

	public:
		void setData(LightData* data)
		{
			m_Data = data;
		}

		DirectionalLight* getDirectionalLight() const { return m_Data->directionalLight; }
		void setDirectionalLight(DirectionalLight* val) { m_Data->directionalLight = val; }

		void addPointLight(PointLight* light);
		std::vector<PointLight*>& getPointLights() { return m_Data->pointLights; }

	private:
		LightData* m_Data;
	};

	using LightMgr = SG<LightManager>;
}
