#include "CameraManager.h"
#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"

namespace tezcat::Tiny::Core
{
	CameraManager::CameraManager()
		: mData(nullptr)
	{
		CameraMgr::attach(this);
	}

	void CameraManager::addCamera(Camera* camera)
	{
		mData->addCamera(camera);
		eventCameraAdded.dispatch(camera);
	}

	void CameraManager::setMainCamera(Camera* camera)
	{
		mData->setMain(camera);
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
		eventSceneChanged.dispatch();
	}


	// CameraData
	Camera* CameraData::getMainCamera()
	{
		return mMain;
	}

	const std::vector<Camera*>& CameraData::getAllCamera()
	{
		if (mDirty)
		{
			mDirty = false;
			std::sort(mCameraList.begin(), mCameraList.end(), [](Camera a, Camera b)->auto
			{
				return a.getDeep() < b.getDeep();
			});
		}

		return mCameraList;
	}

	const std::unordered_map<std::string, Camera*>& CameraData::getCameraMap()
	{
		return mCameraWithName;
	}

	void CameraData::addCamera(Camera* camera)
	{
		if (camera->isMain())
		{
			mMain = camera;
		}

		if (mCameraList.empty())
		{
			mCameraList.push_back(camera);
		}
		else
		{
			auto it = mCameraList.begin();
			auto end = mCameraList.end();
			while (it != end)
			{
				if ((*it)->getDeep() > camera->getDeep())
				{
					mCameraList.insert(it, camera);
					break;
				}
				it++;
			}
		}
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
			std::sort(mCameraList.begin(), mCameraList.end(), [](Camera a, Camera b)->auto
			{
				return a.getDeep() < b.getDeep();
			});
		}
	}

	void CameraData::setMain(Camera* camera)
	{
		if (mMain != nullptr)
		{
			mMain->clearMain();
		}

		mMain = camera;
	}
}
