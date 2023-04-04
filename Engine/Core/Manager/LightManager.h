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
			mData = data;
		}

		DirectionalLight* getDirectionalLight() const { return mData->directionalLight; }
		void setDirectionalLight(DirectionalLight* val) { mData->directionalLight = val; }

		void addPointLight(PointLight* light);
		std::vector<PointLight*>& getPointLights() { return mData->pointLights; }

	private:
		LightData* mData;
	};

	using LightMgr = SG<LightManager>;
}
