#pragma once
#include "../Head/ConfigHead.h"
#include "../Head/RenderConfig.h"
#include "../Head/GLMHead.h"

#include "../Shader/Uniform.h"
#include "../Tool/Tool.h"

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
	class TextureRender2D;
	class FrameBuffer;
	class Vertex;
	class VertexBuffer;
	class IndexBuffer;
	class Image;
	class TextureCube;
	class Texture2D;
	class Transform;
	class Material;
	class GUI;

	class ShadowCasterManager;
	class EnvironmentLightManager;
	class CameraManager;
	class LightManager;
	class BaseQueue;
	class ExtraQueue;
	class RenderCommand;
	class BuildCommand;

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

		void buildCMD();

		virtual void postRender();

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

		//----------------------------------------------------
		//
		//	Command
		//
	public:
		virtual void cmdCreateVertex(Vertex* vertex) = 0;
		virtual void cmdCreateVertexBuffer(VertexBuffer* vertexBuffer) = 0;
		virtual void cmdCreateIndexBuffer(IndexBuffer* indexBuffer) = 0;
		virtual void cmdCreateTexture2D(Texture2D* tex2d) = 0;
		virtual void cmdCreateTextureCube(TextureCube* texCube) = 0;
		virtual void cmdCreateRender2D(TextureRender2D* render2d) = 0;
		virtual void cmdCreateFrameBuffer(FrameBuffer* frameBuffer) = 0;
		virtual void cmdCreateShader(Shader* shader) = 0;
		virtual void cmdCreateShader(Shader* shader, std::string& data) =0;

	public:
		virtual void cmdUpdateTexture2D(Texture2D* tex2d) = 0;

	public:
		virtual void cmdDeleteTexture2D(uint32_t id) = 0;
		virtual void cmdDeleteTextureCube(uint32_t id) = 0;
		virtual void cmdDeleteRender2D(uint32_t id) = 0;
		virtual void cmdDeleteVertex(uint32_t id) = 0;
		virtual void cmdDeleteVertexBuffer(uint32_t id) = 0;
		virtual void cmdDeleteIndexBuffer(uint32_t id) = 0;
		virtual void cmdDeleteFrameBuffer(uint32_t id) = 0;
		virtual void cmdDeleteShader(uint32_t id) = 0;

	public:
		virtual RenderCommand* createDrawVertexCMD(Shader* shader, Vertex* vertex) = 0;
		virtual RenderCommand* createDrawShadowCMD(Vertex* vertex, Transform* transform) = 0;
		virtual RenderCommand* createDrawMeshCMD(Vertex* vertex, Transform* transform, Material* material) = 0;
		virtual RenderCommand* createDrawSkyboxCMD(Shader* shader
												 , Vertex* vertex
												 , TextureCube* cube
												 , float lod = 0
												 , bool isHdr = false) = 0;
		virtual RenderCommand* createDrawHDRToCubeCMD(Shader* shader
													, Vertex* vertex
													, Texture2D* hdr
													, TextureCube* cube) = 0;
		virtual RenderCommand* createDrawEnvMakeIrradiance(Shader* shader
														 , Vertex* vertex
														 , TextureCube* cube
														 , TextureCube* irradiance) = 0;
		virtual RenderCommand* createDrawEnvMakePrefilter(Shader* shader
														, Vertex* vertex
														, TextureCube* cube
														, TextureCube* prefitler
														, uint32_t mipMaxLevel
														, uint32_t mipWidth
														, uint32_t mipHeight
														, float resolution) = 0;

		//----------------------------------------------------
		//
		//	Upload Data
		//
	public:
		virtual void setFloat1(Shader* shader, const char* name, float* data) = 0;
		virtual void setFloat2(Shader* shader, const char* name, float* data) = 0;
		virtual void setFloat3(Shader* shader, const char* name, float* data) = 0;
		virtual void setFloat4(Shader* shader, const char* name, float* data) = 0;

		virtual void setInt1(Shader* shader, const char* name, int* data) = 0;
		virtual void setInt2(Shader* shader, const char* name, int* data) = 0;
		virtual void setInt3(Shader* shader, const char* name, int* data) = 0;
		virtual void setInt4(Shader* shader, const char* name, int* data) = 0;

		virtual void setMat3(Shader* shader, const char* name, float* data) = 0;
		virtual void setMat4(Shader* shader, const char* name, const float* data) = 0;

		virtual void setFloat1(Shader* shader, UniformID& uniform, float* data) = 0;
		virtual void setFloat2(Shader* shader, UniformID& uniform, float* data) = 0;
		virtual void setFloat2(Shader* shader, UniformID& uniform, const glm::vec2& data) = 0;
		virtual void setFloat3(Shader* shader, UniformID& uniform, float* data) = 0;
		virtual void setFloat3(Shader* shader, UniformID& uniform, const glm::vec3& data) = 0;
		virtual void setFloat4(Shader* shader, UniformID& uniform, float* data) = 0;

		virtual void setInt1(Shader* shader, UniformID& uniform, const int& data) = 0;
		virtual void setInt1(Shader* shader, UniformID& uniform, int* data) = 0;
		virtual void setInt2(Shader* shader, UniformID& uniform, int* data) = 0;
		virtual void setInt3(Shader* shader, UniformID& uniform, int* data) = 0;
		virtual void setInt4(Shader* shader, UniformID& uniform, int* data) = 0;

		virtual void setMat3(Shader* shader, UniformID& uniform, float* data) = 0;
		virtual void setMat3(Shader* shader, UniformID& uniform, const glm::mat3& mat3) = 0;
		virtual void setMat4(Shader* shader, UniformID& uniform, const float* data) = 0;
		virtual void setMat4(Shader* shader, UniformID& uniform, const glm::mat4& mat4) = 0;
		virtual void setMat4(Shader* shader, UniformID& uniform, glm::mat4 data[], int count) = 0;

		virtual void setGlobalTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) = 0;
		virtual void setTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) = 0;

		virtual void setGlobalTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) = 0;
		virtual void setTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) = 0;

		//----------------------------------------------------
		//
		//	Set Data
		//
	public:
		virtual void clear(const ClearOption& option) = 0;
		virtual void setViewport(const ViewportInfo& info) = 0;
		virtual void setPassState(Shader* shader) = 0;
		virtual void bind(Shader* shader) = 0;
		virtual void bind(FrameBuffer* frameBuffer) = 0;

		//----------------------------------------------------
		//
		//	Draw Data
		//
	public:
		virtual void draw(Vertex* vertex) = 0;

		virtual void drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color = glm::vec3(0.0f, 1.0f, 0.0f));


	private:
		std::vector<Pipeline*> mPipelineAry;
		std::unordered_map<std::string, uint32_t> mPipelineUMap;


	private:
		ShadowCasterManager* mShadowCasterManager;
		EnvironmentLightManager* mEnvLitManager;
		CameraManager* mCameraManager;
		LightManager* mLightManager;

	protected:
		std::vector<BuildCommand*> mBuildCmdAry;


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
	public:

		GUI* mGUI;
	};

	using Graphics = SG<BaseGraphics>;
}

