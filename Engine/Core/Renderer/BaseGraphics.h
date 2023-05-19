#pragma once
#include "../Head/ConfigHead.h"
#include "../Head/RenderConfig.h"
#include "../Head/GLMHead.h"

#include "../Shader/Uniform.h"
#include "../Tool/Tool.h"
#include "../Head/RenderConfig.h"

namespace tezcat::Tiny
{
	class Vertex;
	class MeshRenderer;
	class Engine;
	class RenderLayer;
	class IRenderObject;
	class IRenderMesh;
	class Camera;
	class Pipeline;

	class ShadowCasterManager;
	class EnvironmentLightManager;
	class CameraManager;
	class LightManager;
	class BaseQueue;
	class ExtraQueue;

	struct ViewportInfo;

	/*
	* BaseGraphics
	*
	* @brief Graphics负责统筹整个引擎的渲染流程,不再需要单独分开
	* @brief 整个流程大体分为三个部分
	*        PreRender
	*        CoreRender
	*        PostRender
	* @brief 
	*/
	class TINY_API BaseGraphics
	{

	public:
		BaseGraphics();
		virtual ~BaseGraphics() = 0;

	public:
		void render();

	public:
		virtual void init(Engine* engine);

		virtual void preRender();
		virtual void onRender();
		virtual void postRender();

	public:
		virtual void clear(const ClearOption& option) = 0;
		virtual void setViewport(const ViewportInfo& info) = 0;

	public:
		virtual void draw(Vertex* vertex) = 0;

	public:
		virtual void drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color = glm::vec3(0.0f, 1.0f, 0.0f));
		void setPipeline(RenderPhase type, const std::string& name, Pipeline* pl);

		LightManager* getLightManager() { return mLightManager; }
		ShadowCasterManager* getShadowCasterManager() { return mShadowCasterManager; }
		EnvironmentLightManager* getEnvLitManager() { return mEnvLitManager; }

		void addBaseRenderPassQueue(BaseQueue* queue)
		{
			mBaseQueue.push_back(queue);
		}

		void addPreRenderPassQueue(ExtraQueue* queue)
		{
			mPreQueue.push_back(queue);
		}

		void addPostRenderPassQueue(ExtraQueue* queue)
		{
			mPostQueue.push_back(queue);
		}

	private:
		std::vector<Pipeline*> mPipelineAry;
		std::unordered_map<std::string, uint32_t> mPipelineUMap;


	private:
		ShadowCasterManager* mShadowCasterManager;
		EnvironmentLightManager* mEnvLitManager;
		CameraManager* mCameraManager;
		LightManager* mLightManager;

	private:
		/// <summary>
		/// 无排序,每个queue独立工作
		/// </summary>
		std::vector<ExtraQueue*> mPreQueue;

		/// <summary>
		/// 基本渲染通道
		/// 每一个对应一个Camera
		/// 按早Order排序
		/// </summary>
		std::vector<BaseQueue*> mBaseQueue;

		/// <summary>
		/// 无排序,每个queue独立工作
		/// </summary>
		std::vector<ExtraQueue*> mPostQueue;
	};

	using Graphics = SG<BaseGraphics>;
}

