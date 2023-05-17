#include "LightManager.h"
#include "../Component/Light.h"

namespace tezcat::Tiny
{
	//------------------------------------------------------------
	TINY_RTTI_CPP(LightData);

	LightData::LightData()
	{

	}

	LightData::~LightData()
	{

	}

	//------------------------------------------------------------
	LightManager::LightManager()
		: mData()
	{
		LightMgr::attach(this);
	}

	LightManager::~LightManager()
	{

	}

	void LightManager::addPointLight(PointLight* light)
	{
		mData->pointLights.push_back(light);
	}
}
