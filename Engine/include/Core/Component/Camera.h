#pragma once
#include "Component.h"

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/CameraObserver.h"

namespace tezcat::Tiny
{
	class Shader;
	class Renderer;
	class BaseGraphics;
	class Pipeline;
	class CameraObserver;
	class FrameBuffer;

	/*
	* Camera
	*
	* @brief 相机模块,负责在渲染流程中查找可见单位,以及提供ViewMatrix的数据
	* @brief 相机可以把自己看到
	*/
	class TINY_API Camera : public ComponentAutoID<Camera>
	{
		friend class CameraData;

		//创建一个forward主相机
		Camera();
		//创建一个forward相机
		Camera(bool mainCamera);

		TINY_OBJECT_H(Camera, ComponentAutoID<Camera>)
		TINY_RENDER_OBSERVER_FUNCTION(mCameraAgent)
	public:
		virtual ~Camera();

	public:
		bool isMain() const;
		void setMain();
		void clearMain();


		TinyUID getCameraUID() { return mCameraAgent->getCameraUID(); }

	private:
		void setCameraUID(TinyUID uid) { mCameraAgent->setCameraUID(uid); }

	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;
		void onClose() override;
	private:
		CameraObserver* mCameraAgent;
	};
}
