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

#include "Core/Manager/ShadowCasterManager.h"

#include "Core/Component/ShadowCaster.h"
	
#include "Core/Shader/ShaderPackage.h"
#include "Core/Shader/Shader.h"
		
#include "Core/Renderer/RenderObjectCache.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Event/EngineEvent.h"

namespace tezcat::Tiny
{
	Shader* ShadowCasterManager::mShader = nullptr;
	std::queue<uint32_t> ShadowCasterManager::mFreeIDs;
	std::list<TinyWeakRef<ShadowCaster>> ShadowCasterManager::mCasterAry;
	bool ShadowCasterManager::mAdded = false;

	void ShadowCasterManager::init()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_BeforeSceneExit, &mCasterAry, [](const EventData& data)
		{
			mCasterAry.clear();
		});
	}

	uint32_t ShadowCasterManager::add(ShadowCaster* caster)
	{
		mCasterAry.push_back(caster);

		if (!mFreeIDs.empty())
		{
			auto id = mFreeIDs.front();
			return id;
		}

		return int32_t(mCasterAry.size() - 1);
	}

	void ShadowCasterManager::recycle(ShadowCaster* caster)
	{
		mFreeIDs.push(caster->getUID());
	}

	void ShadowCasterManager::submit(Shader* mShader)
	{
		auto it = mCasterAry.begin();
		while (it != mCasterAry.end())
		{
			if (auto ptr = it->lock())
			{
				ptr->submit(mShader);
				it++;
			}
			else
			{
				mCasterAry.erase(it);
			}
		}
	}
}

