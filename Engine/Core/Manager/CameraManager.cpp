#include "CameraManager.h"
#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"

namespace tezcat::Tiny
{
	//CameraData* CameraManager::sEmpty = new CameraData();

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

	//--------------------------------------------------------
	//
	//	CameraData
	//
	TINY_RTTI_CPP(CameraData)
	Camera* CameraData::getMainCamera()
	{
		return mMain;
	}

	const std::vector<Camera*>& CameraData::getAllCamera()
	{
		if (mDirty)
		{
			mDirty = false;
			std::sort(mCameraList.begin(), mCameraList.end(), [](Camera* a, Camera* b)->auto
			{
				return a->getDeep() < b->getDeep();
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
			mDirty = true;
			mCameraList.push_back(camera);
// 			auto it = mCameraList.begin();
// 			auto end = mCameraList.end();
// 			while (it != end)
// 			{
// 				if ((*it)->getDeep() >= camera->getDeep())
// 				{
// 					mCameraList.insert(it, camera);
// 					break;
// 				}
// 				it++;
// 			}
		}

		mCameraWithName.emplace(camera->getGameObject()->getName(), camera);
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
			std::sort(mCameraList.begin(), mCameraList.end(), [](Camera* a, Camera* b)->auto
			{
				return a->getDeep() < b->getDeep();
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

	void CameraData::setMain(const std::string& name)
	{
		auto it = mCameraWithName.find(name);
		if (it != mCameraWithName.end())
		{
			this->setMain(it->second);
		}
	}
}
