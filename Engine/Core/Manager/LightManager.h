#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Tool/Tool.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class DirectionalLight;
	class PointLight;
	class SpotLight;


	class TINY_API LightData : public TinyObject
	{
		LightData();
		~LightData();

		TINY_Factory(LightData)
			TINY_RTTI_H(LightData)
	public:


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

		DirectionalLight* getDirectionalLight()
		{
			if (auto ptr = mData.lock())
			{
				return mData->directionalLight;
			}

			return nullptr;
		}

		void setDirectionalLight(DirectionalLight* val) { mData->directionalLight = val; }

		void addPointLight(PointLight* light);
		std::vector<PointLight*>& getPointLights() { return mData->pointLights; }

	private:
		TinyWeakRef<LightData> mData;
	};

	using LightMgr = SG<LightManager>;
}
