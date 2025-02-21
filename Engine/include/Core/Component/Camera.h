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
	class TINY_API Camera : public ComponentT<Camera>
	{
		//创建一个forward主相机
		Camera();
		//创建一个forward相机
		Camera(bool mainCamera);

		TINY_OBJECT_H(Camera, ComponentT<Camera>)
		TINY_RENDER_OBSERVER_FUNCTION(mCameraAgent)
	public:
		virtual ~Camera();
		uint32 getUID() const { return mUID; }

	public:
		bool isMain() const { return mIsMain; }
		void setMain();
		void clearMain() { mIsMain = false; }

	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;

	private:
		CameraObserver* mCameraAgent;

		bool mIsMain;
		uint32 mUID;
	};
}
