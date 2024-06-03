#include "CameraManager.h"
#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"

#include "../Renderer/RenderObjectCache.h"
#include "../Renderer/BaseGraphics.h"

#include "../Event/EngineEvent.h"

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

	void CameraManager::calculate(BaseGraphics* graphics)
	{
		if (mCurrentData)
		{
			mCurrentData->calculate(graphics);
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

	void CameraData::addRenderObserver(BaseRenderObserver* observer)
	{
		mDirty = true;
		mObserverArray.push_back(observer);
	}

	void CameraData::calculate(BaseGraphics* graphics)
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
					RenderObjectCache::culling(index, graphics, observer);
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
