#include "CameraManager.h"
#include "../Scene/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"

namespace tezcat::Tiny::Core
{
	CameraManager::CameraManager()
		: m_CameraWithName()
		, m_Main(nullptr)
	{
		CameraMgr::attach(this);
	}

	void CameraManager::render()
	{
		for (auto c : m_CameraList)
		{
			c->render();
		}
	}

	void CameraManager::addCamera(Camera* camera)
	{
		if (camera->isMain())
		{
			this->setMainCamera(camera);
		}

		if (m_CameraList.empty())
		{
			m_CameraList.push_back(camera);
		}
		else
		{
			auto it = m_CameraList.begin();
			while (it != m_CameraList.end())
			{
				if ((*it)->getDeep() > camera->getDeep())
				{
					m_CameraList.insert(it, camera);
					break;
				}
				it++;
			}
		}

		eventCameraAdded.dispatch(camera);
	}

	void CameraManager::setMainCamera(Camera* camera)
	{
		if (m_Main != nullptr)
		{
			m_Main->clearMain();
		}

		m_Main = camera;
	}

	Camera* CameraManager::getCamera(int index)
	{
		return m_CameraList[index];
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
	void CameraManager::setCurrentSceneCameras(std::unordered_map<std::string, Camera*>& share)
	{
		eventSceneChanged.dispatch();

		m_CameraWithName = &share;
		m_CameraList.clear();

		auto it = m_CameraWithName->begin();
		while (it != m_CameraWithName->end())
		{
			this->addCamera(it->second);
			it++;
		}
	}
}

