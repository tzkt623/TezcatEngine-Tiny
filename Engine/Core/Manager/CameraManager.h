#pragma once

#include "Utility/Utility.h"
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Camera;
	class TINY_API CameraData
	{
		friend class CameraManager;
	public:
		Camera* getMainCamera();
		const std::vector<Camera*>& getAllCamera();
		const std::unordered_map<std::string, Camera*>& getCameraMap();

		Camera* getCamera(const std::string& name);
		Camera* getCamera(int index);
		void sort();

		void setMain(Camera* camera);
		void addCamera(Camera* camera);
	private:
		bool mDirty;
		Camera* mMain;
		std::vector<Camera*> mCameraList;
		std::unordered_map<std::string, Camera*> mCameraWithName;
	};

	class TINY_API CameraManager
	{
	public:
		Event<Camera*> eventCameraAdded;
		Event<> eventSceneChanged;

	public:
		CameraManager();
		void setCameraData(CameraData* data);

	public:
		Camera* getCamera(int index);
		Camera* getCamera(const std::string& name);
		Camera* getMainCamera() { return mData->getMainCamera(); }
		const std::vector<Camera*>& getSortedCameraAry() { return mData->getAllCamera(); }

		void setMainCamera(Camera* camera);
		void addCamera(Camera* camera);
	private:
		CameraData* mData;
	};

	using CameraMgr = SG<CameraManager>;
}
