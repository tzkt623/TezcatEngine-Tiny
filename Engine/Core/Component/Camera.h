#pragma once
#include "Component.h"

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny
{
	class Shader;
	class BaseGraphics;

	/// <summary>
	/// 相机模块
	/// </summary>
	class TINY_API Camera : public ComponentT<Camera>, public IRenderObserver
	{
		//创建一个forward主相机
		Camera();
		//创建一个forward相机
		Camera(bool mainCamera);
		//使用管线创建一个主相机
		Camera(Pipeline* pipeling);
		//使用管线创建一个相机
		Camera(Pipeline* pipeling, bool mainCamera);

		TINY_Factory(Camera)
		TINY_RTTI_H(Camera)

	public:
		virtual ~Camera();

		RenderObjectType getRenderObjectType() override { return RenderObjectType::Camera; }


		void setClearOption(ClearOption option) { mClearMask = option; }
		const ClearOption getClearOption() const { return mClearMask; }

		Type getViewType() { return mType; }
		uint32_t getID() const { return mUID; }

		void render(BaseGraphics* graphics);
		void submit(Shader* shader) override;
		void submitViewMatrix(Shader* shader) override;
		void beginRender() override;
		void endRender() override;


		Pipeline* getPipeline() const { return mPipeline; }
		void setPipeline(Pipeline* val) { mPipeline = val; }

	public:
		glm::mat4& getViewMatrix() override;

	public:
		bool isMain() const { return mIsMain; }
		void setMain();
		void clearMain() { mIsMain = false; }

		int getDeep() const { return mDepth; }
		void setDepth(int val) { mDepth = val; }
		float getFOV() { return mFOV; }
		float getAspect() { return (float)mViewInfo.Width / (float)mViewInfo.Height; }
		float getNear() { return mNearFace; }
		float getFar() { return mFarFace; }

	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;

	public:
		glm::vec3& getFront() { return mFront; }
		glm::vec3& getUp() { return mUp; }
		glm::vec3& getRight() { return mRight; }
		void yawPitch(float yaw, float pitch, bool constrainPitch = true);
		void roll(float roll);

	private:
		void updateVector();
		void updateTransform(Transform* transform);
		
	private:
		int mDepth;
		bool mIsMain;
		uint32_t mUID;

	private:

		//glm::mat4x4 mViewMatrix;
	private:
		glm::vec3 mFront;
		glm::vec3 mUp;
		glm::vec3 mRight;
		glm::vec3 mWorldUp;
		float mYaw;
		float mPitch;
		float mRoll;
		glm::mat4 mViewMatrix;

		//clear options
	private:
		ClearOption mClearMask;
		Pipeline* mPipeline;
	};
}
