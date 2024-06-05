#include "ShadowCasterManager.h"

#include "../Component/ShadowCaster.h"

#include "../Shader/ShaderPackage.h"
#include "../Shader/Shader.h"

#include "../Renderer/RenderObjectCache.h"
#include "../Renderer/BaseGraphics.h"
#include "../Event/EngineEvent.h"

namespace tezcat::Tiny
{
	Shader* ShadowCasterManager::mShader = nullptr;
	std::queue<uint32> ShadowCasterManager::mFreeIDs;
	std::list<TinyWeakRef<ShadowCaster>> ShadowCasterManager::mCasterAry;
	bool ShadowCasterManager::mAdded = false;

	void ShadowCasterManager::init()
	{
		EngineEvent::get()->addListener(EngineEventID::EE_OnPopScene, &mCasterAry, [](const EventData& data)
		{
			mCasterAry.clear();
		});
	}

	uint32 ShadowCasterManager::add(ShadowCaster* caster)
	{
		mCasterAry.push_back(caster);

		if (!mFreeIDs.empty())
		{
			auto id = mFreeIDs.front();
			return id;
		}

		return mCasterAry.size() - 1;
	}

	void ShadowCasterManager::recycle(ShadowCaster* caster)
	{
		mFreeIDs.push(caster->getUID());
	}

	void ShadowCasterManager::submit(Shader* mShader)
	{
		for (auto& caster : mCasterAry)
		{
			caster->submit(mShader);
		}
	}

}

