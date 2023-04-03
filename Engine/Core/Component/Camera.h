#pragma once
#include "Component.h"

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny::Core
{
	class Camera;
	class TINY_API CameraLayer
	{
	public:
		static void addCamera(Camera* camera);
		static void removeCamera(Camera* camera);

		static void addCamera(Camera* camera, unsigned int mask);
		static bool removeCamera(Camera* camera, unsigned int mask);

	private:
		static int binarySearch(unsigned int mask, int count);

	private:
		static std::vector<Camera*> m_Layers[32];
	};

	struct TINY_API ViewportInfo
	{
		int OX;
		int OY;
		int Width;
		int Height;
	};

	class Transform;
	class MeshRenderer;
	class Shader;
	class Scene;
	class Pipeline;

	/// <summary>
	/// 相机模块
	/// </summary>
	class TINY_API Camera : public ComponentT<Camera>
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

		~Camera();

		void setOrtho(float near, float far);
		void setPerspective(float fov, float near, float far);

		Type getViewType() { return mType; }
		uint32_t getID() const { return mUID; }

		Pipeline* getPipeline() const { return mPipeline; }
		void setPipeline(Pipeline* val) { mPipeline = val; }

		void setCullLayer(uint32_t layer)
		{
			return mCullLayerList.push_back(layer);
		}

		std::vector<uint32_t>& getCullLayerList() { return mCullLayerList; }
		bool frustumCulling(GameObject* go) { return true; }

		void render();
		void submit(Shader* shader);
	public:

		glm::mat4x4& getProjectionMatrix() { return mProjectionMatrix; }
		glm::mat4x4& getViewMatrix() { return mViewMatrix; }


	public:
		bool isMain() const { return mIsMain; }
		void setMain();
		void clearMain() { mIsMain = false; }

		int getDeep() const { return mDeep; }
		void setDeep(int val) { mDeep = val; }
		float getFOV() { return mFOV; }
		float getAspect() { return mAspect; }
		float getNear() { return mNearFace; }
		float getFar() { return mFarFace; }

		void setViewRect(int x, int y, int width, int height);
		ViewportInfo& getViewRect() { return mViewInfo; }
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
		void updateCameraVector();

	private:
		int mDeep;
		bool mIsMain;

		uint32_t mUID;

	private:
		ViewportInfo mViewInfo;

		float mNearFace;
		float mFarFace;
		float mFOV;
		float mAspect;
		Type mType;
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


	private:
		Pipeline* mPipeline;
		std::vector<uint32_t> mCullLayerList;
	};
}
