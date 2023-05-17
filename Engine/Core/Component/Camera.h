#pragma once
#include "Component.h"

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny
{
	class Shader;
	class BaseGraphics;


	/*
	* Camera
	*
	* @brief 相机模块,负责在渲染流程中查找可见单位,以及提供ViewMatrix的数据
	* @brief 相机可以把自己看到
	*/
	class TINY_API Camera : public ComponentT<Camera>, public IRenderObserver
	{
		//创建一个forward主相机
		Camera();
		//创建一个forward相机
		Camera(bool mainCamera);

		TINY_Factory(Camera);
		TINY_RTTI_H(Camera);

	public:
		virtual ~Camera();

		uint32_t getID() const { return mUID; }

		void render(BaseGraphics* graphics);
		void submit(Shader* shader) override;
		void submitViewMatrix(Shader* shader) override;

	public:
		bool isMain() const { return mIsMain; }
		void setMain();
		void clearMain() { mIsMain = false; }

	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;
		
	private:
		bool mIsMain;
		uint32_t mUID;
	};
}
