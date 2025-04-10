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

#include "Core/Renderer/RenderObjectCache.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/PipelineWorker.h"

#include "Core/Event/EngineEvent.h"
#include "Core/Component/FlyController.h"

namespace tezcat::Tiny
{
	CameraData* CameraManager::mCurrentData = nullptr;
	RenderObserver* CameraManager::mEditorObserver = nullptr;

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

	void CameraManager::preRender()
	{
		if (mCurrentData)
		{
			mCurrentData->preRender();
		}
	}

	CameraManager::CameraManager()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_FocusObject, this
			, [](const EventData& data)
			{
				//GameObject* go = (GameObject*)data.userData;
				//if (go)
				//{
				//	auto camera_pos = mCurrentData->getMainCamera()->getTransform()->getWorldPosition();
				//	mCurrentData->getMainCamera()->lookAt(go->getTransform());
				//}
			});
	}

	void CameraManager::removeCamera(Camera* camera)
	{
		mCurrentData->removeCamera(camera);
	}

	bool CameraManager::isMain(const Camera* camera)
	{
		return mCurrentData->mMain.get() == camera;
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

	const TinyWeakRef<Camera>& CameraData::getMainCamera()
	{
		return mMain;
	}

	void CameraData::addCamera(Camera* camera)
	{
		auto it = mCameraWithName.find(camera->getGameObject()->getName());
		if (it != mCameraWithName.end())
		{
			throw std::runtime_error(std::format("CameraManager : The same name [{}]", it->first));
		}

		auto observer = (CameraObserver*)camera->getRenderObserver();
		if (!mFreeIDs.empty())
		{
			observer->mUID = mFreeIDs.front();
			mFreeIDs.pop();
		}
		else
		{
			observer->mUID = mRuningArray.size();
		}

		mRuningArray.push_back(observer);
		mCameraWithName.emplace(camera->getGameObject()->getName(), camera);
		mDirty = true;
	}

	void CameraData::removeCamera(Camera* camera)
	{
		 auto uid = camera->getCameraUID();
		 mFreeIDs.push(uid);
		 mCameraWithName.erase(camera->getGameObject()->getName());
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
		auto ptr = mMain.lock();
		if (ptr == camera)
		{
			return;
		}

		if (ptr)
		{
			ptr->getGameObject()->removeComponent<FlyController>();
		}

		mMain.reset(camera);
		if (camera->getGameObject()->getComponent<FlyController>() == nullptr)
		{
			camera->getGameObject()->addComponent<FlyController>();
		}

		EngineEvent::getInstance()->dispatch({ EngineEventID::EE_SetMainCamera, camera });
	}

	void CameraData::setMain(const std::string& name)
	{
		auto it = mCameraWithName.find(name);
		if (it != mCameraWithName.end())
		{
			this->setMain(it->second);
		}
	}

	/*
	* 一个可渲染对象,利用他自己所在layer的ID与自身material中shader的ID
	* 来创建/获得一个observerpipelinepass,并且生成相应的渲染命令
	* 此pass在渲染时,需要知道哪个相机进入进行渲染
	* 需要每帧设置当前相机的信息
	*/
	void CameraData::preRender()
	{
		if (mDirty)
		{
			mDirty = false;
			std::ranges::sort(mRuningArray, [](TinyWeakRef<CameraObserver>& a, TinyWeakRef<CameraObserver>& b)
			{
				return a->getSortingID() < b->getSortingID();
			});
		}

		auto it = mRuningArray.begin();
		auto end = mRuningArray.end();
		while (it != end)
		{
			if (auto observer = it->lock())
			{
				if (!observer->getEnable())
				{
					it++;
					continue;
				}

				observer->preRender();

				//先剔除
				for (auto& index : observer->getCullLayerList())
				{
					//剔除到对应的渲染通道
					RenderObjectCache::culling(index, observer);
				}

				observer->getPipelineQueue()->addToPipeline();

				it++;
			}
			else
			{
				it = mRuningArray.erase(it);
				end = mRuningArray.end();
			}
		}
	}
}
