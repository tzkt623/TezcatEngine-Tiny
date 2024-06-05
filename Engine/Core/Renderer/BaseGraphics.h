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
	class RenderObjectCache;
	class BaseRenderer;
	class BaseMeshRenderer;
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

	class ShadowCasterManager;
	class LightingManager;
	class CameraManager;
	class LightManager;
	class BaseQueue;
	class ExtraQueue;
	class RenderCommand;
	class BuildCommand;
	class Pipeline;
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
		virtual void init(Engine* engine);

		void buildCMD();

		template<class CMD, typename... Args>
		static CMD* createCMD(Args&&... args)
		{
			return new CMD(std::forward<Args>(args)...);
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
		virtual void cmdCreateShader(Shader* shader, std::string& data) = 0;

	public:
		virtual void cmdUpdateTexture2D(Texture2D* tex2d) = 0;

	public:
		virtual void cmdDeleteTexture2D(uint32 id) = 0;
		virtual void cmdDeleteTextureCube(uint32 id) = 0;
		virtual void cmdDeleteRender2D(uint32 id) = 0;
		virtual void cmdDeleteVertex(uint32 id) = 0;
		virtual void cmdDeleteVertexBuffer(uint32 id) = 0;
		virtual void cmdDeleteIndexBuffer(uint32 id) = 0;
		virtual void cmdDeleteFrameBuffer(uint32 id) = 0;
		virtual void cmdDeleteShader(uint32 id) = 0;

	public:
		virtual RenderCommand* createDrawVertexCMD(Vertex* vertex) = 0;
		virtual RenderCommand* createDrawShadowCMD(Vertex* vertex, Transform* transform) = 0;
		virtual RenderCommand* createDrawMeshCMD(Vertex* vertex, Transform* transform, Material* material) = 0;
		virtual RenderCommand* createDrawSkyboxCMD(Vertex* vertex
												 , TextureCube* cube
												 , float lod = 0
												 , bool isHdr = false
												 , float exposure = 1) = 0;
		virtual RenderCommand* createDrawHDRToCubeCMD(Vertex* vertex
													, Texture2D* hdr
													, TextureCube* cube) = 0;
		virtual RenderCommand* createDrawEnvMakeIrradiance(Vertex* vertex
														 , TextureCube* cube
														 , TextureCube* irradiance) = 0;
		virtual RenderCommand* createDrawEnvMakePrefilter(Vertex* vertex
														, TextureCube* cube
														, TextureCube* prefitler
														, uint32 mipMaxLevel
														, uint32 mipWidth
														, uint32 mipHeight
														, float resolution) = 0;

		//----------------------------------------------------
		//
		//	Upload Data
		//
	public:
		virtual void setBool(Shader* shader, const char* name, const bool& data) = 0;

		virtual void setFloat1(Shader* shader, const char* name, const float& data) = 0;
		virtual void setFloat1(Shader* shader, const char* name, float* data) = 0;
		virtual void setFloat2(Shader* shader, const char* name, float* data) = 0;
		virtual void setFloat3(Shader* shader, const char* name, float* data) = 0;
		virtual void setFloat4(Shader* shader, const char* name, float* data) = 0;

		virtual void setInt1(Shader* shader, const char* name, const int& data) = 0;
		virtual void setInt1(Shader* shader, const char* name, int* data) = 0;
		virtual void setInt2(Shader* shader, const char* name, int* data) = 0;
		virtual void setInt3(Shader* shader, const char* name, int* data) = 0;
		virtual void setInt4(Shader* shader, const char* name, int* data) = 0;

		virtual void setMat3(Shader* shader, const char* name, float* data) = 0;
		virtual void setMat4(Shader* shader, const char* name, const float* data) = 0;

		//------------------------------------------------
		//
		//	Global Interface
		//
	public:
		virtual void setFloat1(Shader* shader, UniformID& uniform, float* data) = 0;
		virtual void setFloat2(Shader* shader, UniformID& uniform, float* data) = 0;
		virtual void setFloat2(Shader* shader, UniformID& uniform, const float2& data) = 0;
		virtual void setFloat3(Shader* shader, UniformID& uniform, float* data) = 0;
		virtual void setFloat3(Shader* shader, UniformID& uniform, const float3& data) = 0;
		virtual void setFloat4(Shader* shader, UniformID& uniform, float* data) = 0;

		virtual void setInt1(Shader* shader, UniformID& uniform, const int& data) = 0;
		virtual void setInt1(Shader* shader, UniformID& uniform, int* data) = 0;
		virtual void setInt2(Shader* shader, UniformID& uniform, int* data) = 0;
		virtual void setInt3(Shader* shader, UniformID& uniform, int* data) = 0;
		virtual void setInt4(Shader* shader, UniformID& uniform, int* data) = 0;

		virtual void setMat3(Shader* shader, UniformID& uniform, float* data) = 0;
		virtual void setMat3(Shader* shader, UniformID& uniform, const float3x3& mat3) = 0;
		virtual void setMat4(Shader* shader, UniformID& uniform, const float* data) = 0;
		virtual void setMat4(Shader* shader, UniformID& uniform, const float4x4& mat4) = 0;
		virtual void setMat4(Shader* shader, UniformID& uniform, float4x4 data[], int count) = 0;

		virtual void setGlobalTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) = 0;
		virtual void setTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) = 0;

		virtual void setGlobalTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) = 0;
		virtual void setTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) = 0;


		//-------------------------------------------------------
		//
		//	User Interface
		//
	public:
		virtual void setFloat1(Shader* shader, const int& shaderID, const float& data) = 0;
		virtual void setFloat1(Shader* shader, const int& shaderID, const float* data) = 0;
		virtual void setFloat2(Shader* shader, const int& shaderID, const float* data) = 0;
		virtual void setFloat3(Shader* shader, const int& shaderID, const float* data) = 0;
		virtual void setFloat4(Shader* shader, const int& shaderID, const float* data) = 0;

		virtual void setInt1(Shader* shader, const int& shaderID, const int& data) = 0;
		virtual void setInt1(Shader* shader, const int& shaderID, int* data) = 0;
		virtual void setInt2(Shader* shader, const int& shaderID, int* data) = 0;
		virtual void setInt3(Shader* shader, const int& shaderID, int* data) = 0;
		virtual void setInt4(Shader* shader, const int& shaderID, int* data) = 0;

		virtual void setMat3(Shader* shader, const int& shaderID, const float* data) = 0;
		virtual void setMat3(Shader* shader, const int& shaderID, const float3x3& mat3) = 0;
		virtual void setMat4(Shader* shader, const int& shaderID, const float* data) = 0;
		virtual void setMat4(Shader* shader, const int& shaderID, const float4x4& mat4) = 0;
		virtual void setMat4(Shader* shader, const int& shaderID, float4x4 data[], int count) = 0;

		virtual void setGlobalTexture2D(Shader* shader, const int& shaderID, Texture2D* data) = 0;
		virtual void setTexture2D(Shader* shader, const int& shaderID, Texture2D* data) = 0;

		virtual void setGlobalTextureCube(Shader* shader, const int& shaderID, TextureCube* data) = 0;
		virtual void setTextureCube(Shader* shader, const int& shaderID, TextureCube* data) = 0;

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
		virtual void drawLine(const float3& begin, const float3& end, const float3& color = float3(0.0f, 1.0f, 0.0f));

	protected:
		std::vector<BuildCommand*> mBuildCmdAry;
	};

	using Graphics = SG<BaseGraphics>;


	class TINY_API Graphics2
	{
		Graphics2() = delete;
		~Graphics2() = delete;
	public:
		static void init(Engine* engine, BaseGraphics* agent)
		{
			mAgent = agent;
			mAgent->init(engine);
		}


		static void draw(Vertex* vertex, const std::vector<Uniform*> uniforms);
		static void draw(Vertex* vertex, Material* material);
		static void draw(Vertex* vertex) { mAgent->draw(vertex); }

		static void bind(Shader* shader) { mAgent->bind(shader); }
		static void bind(FrameBuffer* frameBuffer) { mAgent->bind(frameBuffer); }

	public:
		static void cmdCreateVertex(Vertex* vertex) { mAgent->cmdCreateVertex(vertex); }
		static void cmdCreateVertexBuffer(VertexBuffer* vertexBuffer) { mAgent->cmdCreateVertexBuffer(vertexBuffer); }
		static void cmdCreateIndexBuffer(IndexBuffer* indexBuffer) { mAgent->cmdCreateIndexBuffer(indexBuffer); }
		static void cmdCreateTexture2D(Texture2D* tex2D) { mAgent->cmdCreateTexture2D(tex2D); }
		static void cmdCreateTextureCube(TextureCube* texCube) { mAgent->cmdCreateTextureCube(texCube); }
		static void cmdCreateRender2D(TextureRender2D* render2d) { mAgent->cmdCreateRender2D(render2d); }
		static void cmdCreateFrameBuffer(FrameBuffer* frameBuffer) { mAgent->cmdCreateFrameBuffer(frameBuffer); }
		static void cmdCreateShader(Shader* shader) { mAgent->cmdCreateShader(shader); }
		static void cmdCreateShader(Shader* shader, std::string& data) { mAgent->cmdCreateShader(shader, data); }

	public:
		static void cmdUpdateTexture2D(Texture2D* tex2d) { mAgent->cmdUpdateTexture2D(tex2d); }
	public:
		static void cmdDeleteTexture2D(uint32 id) { mAgent->cmdDeleteTexture2D(id); }
		static void cmdDeleteTextureCube(uint32 id) { mAgent->cmdDeleteTextureCube(id); }
		static void cmdDeleteRender2D(uint32 id) { mAgent->cmdDeleteRender2D(id); }
		static void cmdDeleteVertex(uint32 id) { mAgent->cmdDeleteVertex(id); }
		static void cmdDeleteVertexBuffer(uint32 id) { mAgent->cmdDeleteVertexBuffer(id); }
		static void cmdDeleteIndexBuffer(uint32 id) { mAgent->cmdDeleteIndexBuffer(id); }
		static void cmdDeleteFrameBuffer(uint32 id) { mAgent->cmdDeleteFrameBuffer(id); }
		static void cmdDeleteShader(uint32 id) { mAgent->cmdDeleteShader(id); }


	public:
		static RenderCommand* createDrawVertexCMD(Vertex* vertex)
		{
			mAgent->createDrawVertexCMD(vertex);
		}

		static RenderCommand* createDrawShadowCMD(Vertex* vertex, Transform* transform)
		{
			mAgent->createDrawShadowCMD(vertex, transform);
		}
		static RenderCommand* createDrawMeshCMD(Vertex* vertex, Transform* transform, Material* material)
		{
			mAgent->createDrawMeshCMD(vertex, transform, material);
		}

		static RenderCommand* createDrawSkyboxCMD(Vertex* vertex
												 , TextureCube* cube
												 , float lod = 0
												 , bool isHdr = false
												 , float exposure = 1)
		{
			mAgent->createDrawSkyboxCMD(vertex, cube, lod, isHdr, exposure);
		}

		static RenderCommand* createDrawHDRToCubeCMD(Vertex* vertex
													, Texture2D* hdr
													, TextureCube* cube)
		{
			mAgent->createDrawHDRToCubeCMD(vertex, hdr, cube);
		}
		static RenderCommand* createDrawEnvMakeIrradiance(Vertex* vertex
														 , TextureCube* cube
														 , TextureCube* irradiance)
		{
			mAgent->createDrawEnvMakeIrradiance(vertex, cube, irradiance);
		}
		static RenderCommand* createDrawEnvMakePrefilter(Vertex* vertex
														, TextureCube* cube
														, TextureCube* prefitler
														, uint32 mipMaxLevel
														, uint32 mipWidth
														, uint32 mipHeight
														, float resolution)
		{
			mAgent->createDrawEnvMakePrefilter(vertex, cube, prefitler, mipMaxLevel, mipWidth, mipHeight, resolution);
		}


	public:
		template<class CMD, typename... Args>
		CMD* createCMD(Args&&... args)
		{
			return new CMD(std::forward<Args>(args)...);
		}

	private:
		static BaseGraphics* mAgent;
	};
}

