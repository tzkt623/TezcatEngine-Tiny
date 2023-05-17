#pragma once

#include "../Tool/Tool.h"
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Camera;
	class BaseGraphics;

	class TINY_API CameraData : public TinyObject
	{
		friend class CameraManager;
		CameraData() = default;
		TINY_Factory(CameraData);
		TINY_RTTI_H(CameraData);

	public:
		Camera* getMainCamera();
		const std::vector<Camera*>& getAllCamera();
		const std::unordered_map<std::string, Camera*>& getCameraMap();

		Camera* getCamera(const std::string& name);
		Camera* getCamera(int index);
		void sort();

		void setMain(Camera* camera);
		void setMain(const std::string& name);
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
		CameraManager();
		void setCameraData(CameraData* data);
		TinyWeakRef<CameraData>& getCameraData()
		{
			return mData;
		}

	public:
		Camera* getCamera(int index);
		Camera* getCamera(const std::string& name);
		Camera* getMainCamera() { return mData->getMainCamera(); }
		const std::vector<Camera*>& getSortedCameraAry();

		void setMainCamera(Camera* camera);
		void setMainCamera(const std::string& name);
		void addCamera(Camera* camera);

		void calculate(BaseGraphics* graphics);

	private:
		TinyWeakRef<CameraData> mData;
		std::vector<Camera*> mEmptyCamera;

	};

	using CameraMgr = SG<CameraManager>;
}
