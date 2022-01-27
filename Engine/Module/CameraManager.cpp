#include "CameraManager.h"
#include "Camera.h"

namespace tezcat::Tiny::Module
{
	IMPLEMENT_DELEGATE_INSTANCE(CameraManager);

	CameraManager::CameraManager()
	{
		REGISTER_DELEGATE_INSTANCE(CameraManager);
	}

	void CameraManager::addCamera(Camera* camera)
	{
		auto it = m_CameraList.begin();
		while (it != m_CameraList.end())
		{
			if ((*it)->getDeep() > camera->getDeep())
			{
				m_CameraList.insert(it, camera);
				break;
			}
		}

		m_CameraList.push_back(camera);
	}



	Camera* CameraManager::getCamera(const std::string& name)
	{
		auto it = m_CameraWithName->find(name);
		if (it != m_CameraWithName->end())
		{
			return it->second;
		}

		return nullptr;
	}

	void CameraManager::foreach(const std::function<void(Camera* camera)>& function)
	{
		for (auto it : m_CameraList)
		{
			function(it);
		}
	}
	void CameraManager::setSharedCameras(std::unordered_map<std::string, Camera*>* share)
	{
		m_CameraWithName = share;
		m_CameraList.clear();

		auto it = m_CameraWithName->begin();
		while (it != m_CameraWithName->end())
		{
			this->addCamera(it->second);
			it++;
		}
	}
}

