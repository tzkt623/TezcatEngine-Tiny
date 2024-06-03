#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Camera;
	class BaseGraphics;
	class BaseRenderObserver;

	class TINY_API CameraData : public TinyObject
	{
		friend class CameraManager;
		CameraData();
		TINY_OBJECT_H(CameraData, TinyObject)

	public:
		Camera* getMainCamera();
		Camera* findCamera(const std::string& name);

		void setMain(Camera* camera);
		void setMain(const std::string& name);

		void addCamera(Camera* camera);
		void addRenderObserver(BaseRenderObserver* observer);

		void calculate(BaseGraphics* graphics);

	private:
		bool mDirty;
		Camera* mMain;
		std::unordered_map<std::string_view, Camera*> mCameraWithName;
		std::vector<TinyWeakRef<BaseRenderObserver>> mObserverArray;
	};

	class TINY_API CameraManager
	{
		CameraManager() = delete;
		~CameraManager() = delete;
	public:
		static void setData(CameraData* data) { mCurrentData = data; }
		static TinyWeakRef<CameraData> getData() { return mCurrentData; }

		static void setMainCamera(Camera* camera);
		static void setMainCamera(const std::string& name);
		static Camera* getMainCamera() { return mCurrentData->getMainCamera(); }

		static Camera* findCamera(const std::string& name);
		static void addCamera(Camera* camera);

	public:
		static void addRenderObserver(BaseRenderObserver* renderObserver);
		static void calculate(BaseGraphics* graphics);

	private:
		static CameraData* mCurrentData;
	};
}
