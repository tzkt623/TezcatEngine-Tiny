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


#include "Core/Manager/CameraManager.h"
#include "Core/Component/GameObject.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Component/Camera.h"

#include "Core/Renderer/RenderObjectCache.h"
#include "Core/Renderer/BaseGraphics.h"

#include "Core/Event/EngineEvent.h"

namespace tezcat::Tiny
{
	CameraData* CameraManager::mCurrentData = nullptr;

	void CameraManager::addCamera(Camera* camera)
	{
		mCurrentData->addCamera(camera);
	}

	void CameraManager::setMainCamera(Camera* camera)
	{
		mCurrentData->setMain(camera);
	}

	void CameraManager::setMainCamera(const std::string& name)
	{
		mCurrentData->setMain(name);
	}

	Camera* CameraManager::findCamera(const std::string& name)
	{
		return mCurrentData->findCamera(name);
	}

	void CameraManager::calculate()
	{
		if (mCurrentData)
		{
			mCurrentData->calculate();
		}
	}

	void CameraManager::addRenderObserver(BaseRenderObserver* renderObserver)
	{
		mCurrentData->addRenderObserver(renderObserver);
	}


	//--------------------------------------------------------
	//
	//	CameraData
	//
	TINY_OBJECT_CPP(CameraData, TinyObject)

	CameraData::CameraData()
		: mMain(nullptr)
		, mDirty(true)
	{

	}

	Camera* CameraData::getMainCamera()
	{
		return mMain;
	}

	void CameraData::addCamera(Camera* camera)
	{
		auto result = mCameraWithName.try_emplace(camera->getGameObject()->getName(), nullptr);
		if (result.second)
		{
			result.first->second = camera;
			this->addRenderObserver(camera->getRenderObserver());
		}

		this->setMain(camera);
	}

	Camera* CameraData::findCamera(const std::string& name)
	{
		auto it = mCameraWithName.find(name);
		if (it != mCameraWithName.end())
		{
			return it->second;
		}

		return nullptr;
	}

	void CameraData::setMain(Camera* camera)
	{
		if (camera == mMain)
		{
			return;
		}

		if (mMain != nullptr)
		{
			mMain->clearMain();
		}

		mMain = camera;
		EngineEvent::getInstance()->dispatch({ EngineEventID::EE_SetMainCamera, mMain });
	}

	void CameraData::setMain(const std::string& name)
	{
		auto it = mCameraWithName.find(name);
		if (it != mCameraWithName.end())
		{
			this->setMain(it->second);
		}
	}

	void CameraData::addRenderObserver(BaseRenderObserver* observer)
	{
		mDirty = true;
		mObserverArray.push_back(observer);
	}

	void CameraData::calculate()
	{
		if (mDirty)
		{
			mDirty = false;
			std::ranges::sort(mObserverArray, [](TinyWeakRef<BaseRenderObserver>& a, TinyWeakRef<BaseRenderObserver>& b)
			{
				return a->getOrderID() < b->getOrderID();
			});
		}

		auto it = mObserverArray.begin();
		while (it != mObserverArray.end())
		{
			if (auto observer = (*it).lock())
			{
				if (!observer->getEnable())
				{
					continue;
				}

				observer->prepareRender();

				if (observer->isNeedRemove())
				{
					observer->onExitPipeline();
					it = mObserverArray.erase(it);
					continue;
				}

				//先剔除
				for (auto& index : observer->getCullLayerList())
				{
					//剔除到对应的渲染通道
					RenderObjectCache::culling(index, observer);
				}
				it++;
			}
			else
			{
				it = mObserverArray.erase(it);
			}
		}
	}
}
