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
		void preRender() override;
		void onRender() override;
		void postRender() override;

		GLFWwindow* getGLFWwindow()
		{
			return mWindow;
		}

	public:

		void setViewport(const ViewportInfo& info) override;
		void clear(const ClearOption& option) override;
		void setPassState(Shader* shader) override;
		void bind(Shader* shader) override;
		void bind(FrameBuffer* frameBuffer) override;

		void draw(Vertex* vertex) override;
		void drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color = glm::vec3(0.0f, 1.0f, 0.0f)) override;
		void drawLine(Vertex* vertex, const uint32_t& needCount);

	public:
		RenderCommand* createDrawVertexCMD(Shader* shader, Vertex* vertex) override;
		RenderCommand* createDrawShadowCMD(Vertex* vertex, Transform* transform) override;
		RenderCommand* createDrawMeshCMD(Vertex* vertex, Transform* transform, Material* material) override;
		RenderCommand* createDrawSkyboxCMD(Shader* shader, Vertex* vertex, TextureCube* cube, float lod = 0, bool isHdr = false) override;
		RenderCommand* createDrawHDRToCubeCMD(Shader* shader, Vertex* vertex, int* unifromIDHDR, Texture2D* hdr, TextureCube* cube) override;
		RenderCommand* createDrawEnvMakeIrradiance(Shader* shader, Vertex* vertex, TextureCube* cube, TextureCube* irradiance) override;
		RenderCommand* createDrawEnvMakePrefilter(Shader* shader, Vertex* vertex, TextureCube* cube, TextureCube* prefitler, uint32_t mipMaxLevel, uint32_t mipWidth, uint32_t mipHeight, float resolution) override;

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
		void setFloat2(Shader* shader, UniformID& uniform, const glm::vec2& data) override;
		void setFloat3(Shader* shader, UniformID& uniform, float* data) override;
		void setFloat3(Shader* shader, UniformID& uniform, const glm::vec3& data) override;
		void setFloat4(Shader* shader, UniformID& uniform, float* data) override;

		void setInt1(Shader* shader, UniformID& uniform, const int& data) override;
		void setInt1(Shader* shader, UniformID& uniform, int* data) override;
		void setInt2(Shader* shader, UniformID& uniform, int* data) override;
		void setInt3(Shader* shader, UniformID& uniform, int* data) override;
		void setInt4(Shader* shader, UniformID& uniform, int* data) override;

		void setMat3(Shader* shader, UniformID& uniform, float* data) override;
		void setMat3(Shader* shader, UniformID& uniform, const glm::mat3& mat3) override;
		void setMat4(Shader* shader, UniformID& uniform, const float* data) override;
		void setMat4(Shader* shader, UniformID& uniform, const glm::mat4& mat4) override;
		void setMat4(Shader* shader, UniformID& uniform, glm::mat4 data[], int count) override;

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
		void setMat3(Shader* shader, const int& shaderID, const glm::mat3& mat3) override;
		void setMat4(Shader* shader, const int& shaderID, const float* data) override;
		void setMat4(Shader* shader, const int& shaderID, const glm::mat4& mat4) override;
		void setMat4(Shader* shader, const int& shaderID, glm::mat4 data[], int count) override;

		void setGlobalTexture2D(Shader* shader, const int& shaderID, Texture2D* data) override;
		void setTexture2D(Shader* shader, const int& shaderID, Texture2D* data) override;
		void setGlobalTextureCube(Shader* shader, const int& shaderID, TextureCube* data) override;
		void setTextureCube(Shader* shader, const int& shaderID, TextureCube* data) override;

	public:
		void cmdCreateVertex(Vertex* vertex) override;
		void cmdCreateVertexBuffer(VertexBuffer* vertexBuffer) override;
		void cmdCreateIndexBuffer(IndexBuffer* indexBuffer) override;
		void cmdCreateTexture2D(Texture2D* tex2d) override;
		void cmdCreateTextureCube(TextureCube* texCube) override;
		void cmdCreateRender2D(TextureRender2D* render2d) override;
		void cmdCreateFrameBuffer(FrameBuffer* frameBuffer) override;
		void cmdCreateShader(Shader* shader) override;
		void cmdCreateShader(Shader* shader, std::string& data) override;

	public:
		void cmdUpdateTexture2D(Texture2D* tex2d) override;


	public:
		void cmdDeleteTexture2D(uint32_t id) override;
		void cmdDeleteTextureCube(uint32_t id) override;
		void cmdDeleteRender2D(uint32_t id) override;
		void cmdDeleteVertex(uint32_t id) override;
		void cmdDeleteVertexBuffer(uint32_t id) override;
		void cmdDeleteIndexBuffer(uint32_t id) override;
		void cmdDeleteFrameBuffer(uint32_t id) override;
		void cmdDeleteShader(uint32_t id) override;

	private:
		void initContext();


		template<class CMD, typename... Args>
		void createCMD(Args&&... args)
		{
			auto cmd = new CMD(std::forward<Args>(args)...);
			if (Engine::sMultiThread)
			{
				mBuildCmdAry.push_back(cmd);
			}
			else
			{
				cmd->execute(this);
				delete cmd;
			}
		}

	private:
		GLFWwindow* mWindow;
	};
}
