#pragma once
#include "Component.h"

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"

#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class BaseGraphics;

	/// <summary>
	/// 相机模块
	/// </summary>
	class TINY_API Camera : public ComponentT<Camera>, public IRenderObserver
	{
	public:
		enum class Type
		{
			Ortho,
			Perspective
		};

	public:
		//创建一个forward主相机
		Camera();
		//创建一个forward相机
		Camera(bool mainCamera);
		//使用管线创建一个主相机
		Camera(Pipeline* pipeling);
		//使用管线创建一个相机
		Camera(Pipeline* pipeling, bool mainCamera);

		virtual ~Camera();

		RenderObjectType getRenderObjectType() override { return RenderObjectType::Camera; }

		void setOrtho(float near, float far);
		void setPerspective(float fov, float near, float far);
		void setClearOption(uint32_t option) { mClearMask = option; }
		const uint32_t getClearOption() const { return mClearMask; }

		Type getViewType() { return mType; }
		uint32_t getID() const { return mUID; }

		void render(BaseGraphics* graphics);
		void submit(Shader* shader) override;

	public:
		glm::mat4x4& getProjectionMatrix() { return mProjectionMatrix; }
		glm::mat4x4& getViewMatrix() { return mViewMatrix; }

	public:
		bool isMain() const { return mIsMain; }
		void setMain();
		void clearMain() { mIsMain = false; }

		int getDeep() const { return mDepth; }
		void setDepth(int val) { mDepth = val; }
		float getFOV() { return mFOV; }
		float getAspect() { return mAspect; }
		float getNear() { return mNearFace; }
		float getFar() { return mFarFace; }

	protected:
		void onStart() override;
		void onEnable() override;
		void onUpdate() override;
		void onDisable() override;

	public:
		glm::vec3& getFront() { return mFront; }
		glm::vec3& getUp() { return mUp; }
		glm::vec3& getRight() { return mRight; }
		void yawPitch(float yaw, float pitch, bool constrainPitch = true);
		void roll(float roll);

	private:
		void updateVector();

	private:
		int mDepth;
		bool mIsMain;
		uint32_t mUID;

	private:
		Type mType;

		float mNearFace;
		float mFarFace;
		float mFOV;
		bool mPMatDirty;

		glm::mat4x4 mProjectionMatrix;
		glm::mat4x4 mViewMatrix;
	private:
		glm::vec3 mFront;
		glm::vec3 mUp;
		glm::vec3 mRight;
		glm::vec3 mWorldUp;
		float mYaw;
		float mPitch;
		float mRoll;

		//clear options
	private:
		uint32_t mClearMask;
	};
}
