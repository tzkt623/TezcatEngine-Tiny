#pragma once
#include <list>
#include <functional>
#include "Tools.h"

namespace tezcat::Tiny::Module
{
	class Camera;
	class CameraManager
	{
		CREATE_DELEGATE_INSTANCE(CameraManager)
	public:
		CameraManager();
		
	public:
		Camera* getCamera(const std::string& name);

		void foreach(const std::function<void(Camera* camera)>& function);
		void setSharedCameras(std::unordered_map<std::string, Camera*>* share);
		void addCamera(Camera* camera);


	private:
		std::list<Camera*> m_CameraList;

		std::unordered_map<std::string, Camera*>* m_CameraWithName;
	};
}