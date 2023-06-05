#include "CameraManager.h"
#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"

#include "../Renderer/RenderLayer.h"
#include "../Renderer/BaseGraphics.h"

#include "../Event/EngineEvent.h"

namespace tezcat::Tiny
{
	CameraManager::CameraManager()
		: mData()
	{
		CameraMgr::attach(this);
	}

	void CameraManager::addCamera(Camera* camera)
	{
		mData->addCamera(camera);
	}

	void CameraManager::setMainCamera(Camera* camera)
	{
		mData->setMain(camera);
	}

	void CameraManager::setMainCamera(const std::string& name)
	{
		mData->setMain(name);
	}

	Camera* CameraManager::getCamera(int index)
	{
		return mData->getCamera(index);
	}

	Camera* CameraManager::getCamera(const std::string& name)
	{
		return mData->getCamera(name);
	}

	void CameraManager::setCameraData(CameraData* data)
	{
		mData = data;
	}

	const std::vector<Camera*>& CameraManager::getSortedCameraAry()
	{
		if (auto ptr = mData.lock())
		{
			return ptr->getAllCamera();
		}

		return mEmptyCamera;
	}

	void CameraManager::calculate(BaseGraphics* graphics)
	{
		if (!mData.lock())
		{
			return;
		}

		auto cameras = mData->getAllCamera();
		for (auto camera : cameras)
		{
			if (camera->isEnable())
			{
				RenderQueue* queue = camera->getRenderQueue();
				graphics->addBaseRenderPassQueue((BaseQueue*)queue);

				//先剔除
				auto& cull_list = camera->getCullLayerList();
				for (auto& index : cull_list)
				{
					//剔除到对应的渲染通道
					RenderLayer::getRenderLayer(index)->culling(graphics, camera, queue);
				}
			}
		}
	}

	//--------------------------------------------------------
	//
	//	CameraData
	//
	TINY_RTTI_CPP(CameraData);
	Camera* CameraData::getMainCamera()
	{
		return mMain;
	}

	const std::vector<Camera*>& CameraData::getAllCamera()
	{
		this->sort();
		return mCameraList;
	}

	const std::unordered_map<std::string, Camera*>& CameraData::getCameraMap()
	{
		return mCameraWithName;
	}

	void CameraData::addCamera(Camera* camera)
	{
		this->setMain(camera);

		if (mCameraList.empty())
		{
			mCameraList.push_back(camera);
		}
		else
		{
			mDirty = true;
			mCameraList.push_back(camera);
		}

		mCameraWithName.try_emplace(camera->getGameObject()->getName(), camera);
	}

	Camera* CameraData::getCamera(const std::string& name)
	{
		auto it = mCameraWithName.find(name);
		if (it != mCameraWithName.end())
		{
			return it->second;
		}

		return nullptr;
	}

	Camera* CameraData::getCamera(int index)
	{
		return mCameraList[index];
	}

	void CameraData::sort()
	{
		if (mDirty)
		{
			mDirty = false;
			std::ranges::sort(mCameraList, [](Camera* a, Camera* b)->auto
			{
				return a->getOrder() < b->getOrder();
			});
		}
	}

	void CameraData::setMain(Camera* camera)
	{
		mDirty = true;

		if (mMain != nullptr && mMain != camera)
		{
			mMain->clearMain();
		}

		mMain = camera;
		EngineEvent::get()->dispatch({ EngineEventID::EE_SetMainCamera, mMain });
	}

	void CameraData::setMain(const std::string& name)
	{
		auto it = mCameraWithName.find(name);
		if (it != mCameraWithName.end())
		{
			this->setMain(it->second);
		}
	}
}
