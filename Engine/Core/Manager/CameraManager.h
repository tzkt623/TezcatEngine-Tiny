#pragma once

#include "Utility/Tools.h"
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Camera;
	class MeshRenderer;
	class TINY_API CameraManager
	{
	public:
		Event<Camera*> eventCameraAdded;
		Event<> eventSceneChanged;

	public:
		CameraManager();
		void render();

	public:
		Camera* getCamera(int index);
		Camera* getCamera(const std::string& name);
		inline Camera* getMainCamera() { return m_Main; }
		void setMainCamera(Camera* camera);

		void foreach(const std::function<void(Camera* camera)>& function);
		void setCurrentSceneCameras(std::unordered_map<std::string, Camera*>& share);
		void addCamera(Camera* camera);

		inline std::vector<Camera*>& getAllCamera() { return m_CameraList; }

	private:
		Camera* m_Main;
		std::vector<Camera*> m_CameraList;
		std::unordered_map<std::string, Camera*>* m_CameraWithName;
	};

	using CameraMgr = SG<CameraManager>;
}