#pragma once

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Head/ConfigHead.h"

struct GLFWwindow;

namespace tezcat::Tiny
{
	class GUI;
}

namespace tezcat::Tiny::GL
{
	class TINY_API GLGraphics : public BaseGraphics
	{
	public:
		GLGraphics();
		virtual ~GLGraphics();

	public:
		void init(Engine* engine) override;
		GLFWwindow* getGLFWwindow() { return mWindow; }

	public:

		void setViewport(const ViewportInfo& info) override;
		void clear(const ClearOption& option) override;
		void setPassState(Shader* shader) override;
		void bind(Shader* shader) override;
		void bind(FrameBuffer* frameBuffer) override;

		void draw(Vertex* vertex) override;
		void drawLine(const float3& begin, const float3& end, const float3& color = float3(0.0f, 1.0f, 0.0f)) override;
		void drawLine(Vertex* vertex, const uint32& needCount);

	public:
		virtual void createVertex(Vertex* vertex) override;
		void createBuffer(VertexBuffer* buffer) override;
		void createBuffer(IndexBuffer* buffer) override;
		void createBuffer(FrameBuffer* buffer) override;
		void createShader(Shader* shader) override;
		void createTexture(Texture2D* tex2D) override;
		void createTexture(TextureCube* texCube) override;
		void createTexture(TextureRender2D* render2D) override;

		virtual void deleteVertex(Vertex* vertex) override;
		virtual void deleteBuffer(VertexBuffer* vertexBuffer) override;
		virtual void deleteBuffer(IndexBuffer* indexBuffer) override;
		virtual void deleteShader(Shader* shader) override;
		virtual void deleteTexture2D(const uint32& id) override;
		virtual void deleteTextureCube(const uint32& id) override;
		virtual void deleteRender2D(const uint32& id) override;

		virtual void makeHDR2Cube(Shader* shader
			, Vertex* vertex
			, Texture2D* texHDR
			, TextureCube* skybox) override;
		virtual void makeEnvIrradiance(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance) override;
		virtual void makeEnvPrefilter(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32 mipMaxLevel
			, uint32 mipWidth
			, uint32 mipHeight
			, int32 resolution) override;

	public:
		//RenderCommand* createDrawVertexCMD(Vertex* vertex) override;
		//RenderCommand* createDrawShadowCMD(Vertex* vertex, Transform* transform) override;
		//RenderCommand* createDrawCMD(Vertex* vertex, Transform* transform, Material* material) override;
		//RenderCommand* createDrawSkyboxCMD(Vertex* vertex, TextureCube* cube, float lod = 0, bool isHdr = false, float exposure = 1) override;
		//RenderCommand* createDrawHDRToCubeCMD(Vertex* vertex, Texture2D* hdr, TextureCube* cube) override;
		//RenderCommand* createDrawEnvMakeIrradiance(Vertex* vertex, TextureCube* cube, TextureCube* irradiance) override;
		//RenderCommand* createDrawEnvMakePrefilter(Vertex* vertex, TextureCube* cube, TextureCube* prefitler, uint32 mipMaxLevel, uint32 mipWidth, uint32 mipHeight, float resolution) override;

	public:
		void setBool(Shader* shader, const char* name, const bool& data) override;

		void setFloat1(Shader* shader, const char* name, const float& data) override;
		void setFloat1(Shader* shader, const char* name, float* data) override;
		void setFloat2(Shader* shader, const char* name, float* data) override;
		void setFloat3(Shader* shader, const char* name, float* data) override;
		void setFloat4(Shader* shader, const char* name, float* data) override;

		void setInt1(Shader* shader, const char* name, const int& data) override;
		void setInt1(Shader* shader, const char* name, int* data) override;
		void setInt2(Shader* shader, const char* name, int* data) override;
		void setInt3(Shader* shader, const char* name, int* data) override;
		void setInt4(Shader* shader, const char* name, int* data) override;

		void setMat3(Shader* shader, const char* name, float* data) override;
		void setMat4(Shader* shader, const char* name, const float* data) override;

		void setFloat1(Shader* shader, UniformID& uniform, float* data) override;
		void setFloat2(Shader* shader, UniformID& uniform, float* data) override;
		void setFloat2(Shader* shader, UniformID& uniform, const float2& data) override;
		void setFloat3(Shader* shader, UniformID& uniform, float* data) override;
		void setFloat3(Shader* shader, UniformID& uniform, const float3& data) override;
		void setFloat4(Shader* shader, UniformID& uniform, float* data) override;

		void setInt1(Shader* shader, UniformID& uniform, const int& data) override;
		void setInt1(Shader* shader, UniformID& uniform, int* data) override;
		void setInt2(Shader* shader, UniformID& uniform, int* data) override;
		void setInt3(Shader* shader, UniformID& uniform, int* data) override;
		void setInt4(Shader* shader, UniformID& uniform, int* data) override;

		void setMat3(Shader* shader, UniformID& uniform, float* data) override;
		void setMat3(Shader* shader, UniformID& uniform, const float3x3& mat3) override;
		void setMat4(Shader* shader, UniformID& uniform, const float* data) override;
		void setMat4(Shader* shader, UniformID& uniform, const float4x4& mat4) override;
		void setMat4(Shader* shader, UniformID& uniform, float4x4 data[], int count) override;

		void setGlobalTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) override;
		void setTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) override;
		void setGlobalTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) override;
		void setTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) override;


	public:
		void setFloat1(Shader* shader, const int& shaderID, const float& data) override;
		void setFloat1(Shader* shader, const int& shaderID, const float* data) override;
		void setFloat2(Shader* shader, const int& shaderID, const float* data) override;
		void setFloat3(Shader* shader, const int& shaderID, const float* data) override;
		void setFloat4(Shader* shader, const int& shaderID, const float* data) override;

		void setInt1(Shader* shader, const int& shaderID, const int& data) override;
		void setInt1(Shader* shader, const int& shaderID, int* data) override;
		void setInt2(Shader* shader, const int& shaderID, int* data) override;
		void setInt3(Shader* shader, const int& shaderID, int* data) override;
		void setInt4(Shader* shader, const int& shaderID, int* data) override;

		void setMat3(Shader* shader, const int& shaderID, const float* data) override;
		void setMat3(Shader* shader, const int& shaderID, const float3x3& mat3) override;
		void setMat4(Shader* shader, const int& shaderID, const float* data) override;
		void setMat4(Shader* shader, const int& shaderID, const float4x4& mat4) override;
		void setMat4(Shader* shader, const int& shaderID, float4x4 data[], int count) override;

		void setGlobalTexture2D(Shader* shader, const int& shaderID, Texture2D* data) override;
		void setTexture2D(Shader* shader, const int& shaderID, Texture2D* data) override;
		void setGlobalTextureCube(Shader* shader, const int& shaderID, TextureCube* data) override;
		void setTextureCube(Shader* shader, const int& shaderID, TextureCube* data) override;

	public:
		void cmdUpdateTexture2D(Texture2D* tex2d) override;

	private:
		void initContext();

	private:
		GLFWwindow* mWindow;
	};
}
