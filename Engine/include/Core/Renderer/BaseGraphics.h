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

	/* CMD */
	class RenderCommadBuild;

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

		void buildCommand();

	public:
		virtual void createVertex(Vertex* vertex) {}
		virtual void createShader(Shader* shader) {}
		virtual void createBuffer(VertexBuffer* buffer) {}
		virtual void createBuffer(IndexBuffer* buffer) {}
		virtual void createBuffer(FrameBuffer* buffer) {}
		virtual void createTexture(Texture2D* tex2D) {}
		virtual void createTexture(TextureCube* texCube) {}
		virtual void createTexture(TextureRender2D* render2D) {}

	public:
		virtual void deleteVertex(Vertex* vertex) {}
		virtual void deleteBuffer(VertexBuffer* vertexBuffer) {}
		virtual void deleteBuffer(IndexBuffer* indexBuffer) {}
		virtual void deleteBuffer(FrameBuffer* frameBuffer) {}
		virtual void deleteShader(Shader* shader) {}
		virtual void deleteTexture2D(const uint32& id) {}
		virtual void deleteTextureCube(const uint32& id) {}
		virtual void deleteRender2D(const uint32& id) {}

	public:
		virtual void makeHDR2Cube(Shader* shader
			, Vertex* vertex
			, Texture2D* texHDR
			, TextureCube* skybox) {}
		virtual void makeEnvIrradiance(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance) {}
		virtual void makeEnvPrefilter(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32 mipMaxLevel
			, uint32 mipWidth
			, uint32 mipHeight
			, int32 resolution) {}

		//----------------------------------------------------
		//
		//	Command
		//
	public:
		virtual void cmdUpdateTexture2D(Texture2D* tex2d) = 0;

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

	public:
		void addCommand(RenderCommadBuild* cmd);

		template<class CMD, typename... Args>
		void addCommand(Args&&... args)
		{
			mBuildCommandList.push_back(new CMD(TINY_FWD(args)...));
		}

	private:
		std::vector<RenderCommadBuild*> mBuildCommandList;
	};

	using Graphics = SG<BaseGraphics>;
}

