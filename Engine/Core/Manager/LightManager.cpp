#include "LightManager.h"
#include "../Component/Light.h"

namespace tezcat::Tiny::Core
{
	//------------------------------------------------------------
	LightData::LightData()
		: directionalLight(nullptr)
	{

	}

	LightData::~LightData()
	{
		delete directionalLight;
		directionalLight = nullptr;

		for (auto l : pointLights)
		{
			delete l;
		}

		for (auto l : spotLights)
		{
			delete l;
		}
	}

	//------------------------------------------------------------
	LightManager::LightManager()
		: m_Data(new LightData())
	{
		SG<LightManager>::attach(this);
	}

	LightManager::~LightManager()
	{

	}

}