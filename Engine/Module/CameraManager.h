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
		Camera* getCamera(int index);
		Camera* getCamera(const std::string& name);
		Camera* getMainCamera();

		void foreach(const std::function<void(Camera* camera)>& function);
		void setSharedCameras(std::unordered_map<std::string, Camera*>* share);
		void addCamera(Camera* camera);

		std::vector<Camera*>& getAllCamera() { return m_CameraList; }

	private:
		std::vector<Camera*> m_CameraList;
		std::unordered_map<std::string, Camera*>* m_CameraWithName;
	};
}