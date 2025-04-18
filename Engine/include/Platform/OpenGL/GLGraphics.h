#pragma once
/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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
		void preRender() override;
		void postRender() override;

	public:
		void setViewport(const ViewportInfo& info) override;
		void clear(const ClearOption& option) override;
		void setPassState(Shader* shader) override;
		void bind(Shader* shader) override;
		void bind(UniformBuffer* uniformBuffer) override;
		void bind(Texture2D* texture) override;
		void readPixel(int32_t x, int32_t y) override;
		void readObjectID(int32_t x, int32_t y, int32_t& id) override;

		void draw(Vertex* vertex) override;
		void drawLine(const float3& begin, const float3& end, const float3& color = float3(0.0f, 1.0f, 0.0f)) override;
		void drawLine(Vertex* vertex, const uint32_t& needCount);

	protected:
		void bindImpl(FrameBuffer* frameBuffer) override;

	public:
		virtual void createVertex(Vertex* vertex) override;
		void createBuffer(VertexBuffer* buffer) override;
		void createBuffer(IndexBuffer* buffer) override;
		void createBuffer(FrameBuffer* buffer) override;
		void createUniformBuffer(UniformBuffer* buffer, int32_t bindingIndex) override;
		void createShader(Shader* shader) override;
		void createTexture(Texture2D* tex2D) override;
		void createTexture(TextureCube* texCube) override;
		void createTexture(TextureRender2D* render2D) override;
		virtual void createMipmapTexCube(TextureCube* texCube, int32_t minLevel, int32_t maxLevel) override;

	public:
		virtual void deleteVertex(const uint32_t& id) override;
		virtual void deleteVertexBuffer(const uint32_t& id) override;
		virtual void deleteIndexBuffer(const uint32_t& id) override;
		virtual void deleteUniformBuffer(const uint32_t& id) override;
		virtual void deleteTexture2D(const uint32_t& id) override;
		virtual void deleteTextureCube(const uint32_t& id) override;
		virtual void deleteRender2D(const uint32_t& id) override;
		virtual void deleteFrameBuffer(const uint32_t& id) override;
		virtual void deleteShader(const uint32_t& id) override;

	public:
		virtual void makeHDR2Cube(Shader* shader
			, BaseRenderObserver* observer
			, Vertex* vertex
			, Texture2D* texHDR
			, TextureCube* skybox
			, std::array<int32_t, 2> viewSize) override;
		virtual void makeEnvIrradiance(Shader* shader
			, BaseRenderObserver* observer
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance
			, std::array<int32_t, 2> viewSize) override;
		virtual void makeEnvPrefilter(Shader* shader
			, BaseRenderObserver* observer
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32_t mipMaxLevel
			, uint32_t mipWidth
			, uint32_t mipHeight
			, int32_t resolution) override;

	public:
		virtual void setUniformBuffer(UniformBuffer* uniformBuffer) override;
		virtual void setFrameBufferColorTexture2D(Texture2D* tex, int32_t index) override;
		virtual void setFrameBufferColorTextureCube(TextureCube* tex, int32_t index, int32_t cubeFace) override;

	public:
		//RenderCommand* createDrawVertexCMD(Vertex* vertex) override;
		//RenderCommand* createDrawShadowCMD(Vertex* vertex, Transform* transform) override;
		//RenderCommand* createDrawCMD(Vertex* vertex, Transform* transform, Material* material) override;
		//RenderCommand* createDrawSkyboxCMD(Vertex* vertex, TextureCube* cube, float lod = 0, bool isHdr = false, float exposure = 1) override;
		//RenderCommand* createDrawHDRToCubeCMD(Vertex* vertex, Texture2D* hdr, TextureCube* cube) override;
		//RenderCommand* createDrawEnvMakeIrradiance(Vertex* vertex, TextureCube* cube, TextureCube* irradiance) override;
		//RenderCommand* createDrawEnvMakePrefilter(Vertex* vertex, TextureCube* cube, TextureCube* prefitler, uint32_t mipMaxLevel, uint32_t mipWidth, uint32_t mipHeight, float resolution) override;

	public:
		void setBool(Shader* shader, const char* name, const bool& data) override;

		void setFloat1(Shader* shader, const char* name, const float& data) override;
		void setFloat1(Shader* shader, const char* name, float* data) override;
		void setFloat2(Shader* shader, const char* name, float* data) override;
		void setFloat3(Shader* shader, const char* name, float* data) override;
		void setFloat4(Shader* shader, const char* name, float* data) override;

		void setUInt1(Shader* shader, const char* name, const uint32_t& data) override;
		void setInt1(Shader* shader, const char* name, const int32_t& data) override;
		void setInt1(Shader* shader, const char* name, int32_t* data) override;
		void setInt2(Shader* shader, const char* name, int32_t* data) override;
		void setInt3(Shader* shader, const char* name, int32_t* data) override;
		void setInt4(Shader* shader, const char* name, int32_t* data) override;

		void setMat3(Shader* shader, const char* name, float* data) override;
		void setMat4(Shader* shader, const char* name, const float* data) override;

		void setFloat1(Shader* shader, UniformID& uniform, float* data) override;
		void setFloat2(Shader* shader, UniformID& uniform, float* data) override;
		void setFloat2(Shader* shader, UniformID& uniform, const float2& data) override;
		void setFloat3(Shader* shader, UniformID& uniform, float* data) override;
		void setFloat3(Shader* shader, UniformID& uniform, const float3& data) override;
		void setFloat4(Shader* shader, UniformID& uniform, float* data) override;



		void setInt1(Shader* shader, UniformID& uniform, const int32_t& data) override;
		void setInt1(Shader* shader, UniformID& uniform, int32_t* data) override;
		void setInt2(Shader* shader, UniformID& uniform, int32_t* data) override;
		void setInt3(Shader* shader, UniformID& uniform, int32_t* data) override;
		void setInt4(Shader* shader, UniformID& uniform, int32_t* data) override;

		void setMat3(Shader* shader, UniformID& uniform, float* data) override;
		void setMat3(Shader* shader, UniformID& uniform, const float3x3& mat3) override;
		void setMat4(Shader* shader, UniformID& uniform, const float* data) override;
		void setMat4(Shader* shader, UniformID& uniform, const float4x4& mat4) override;
		void setMat4(Shader* shader, UniformID& uniform, float4x4 data[], int32_t count) override;

		void setGlobalTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) override;
		void setTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) override;
		void setGlobalTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) override;
		void setTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) override;


	public:
		void setFloat1(Shader* shader, const int32_t& valueID, const float* data) override;
		void setFloat2(Shader* shader, const int32_t& valueID, const float* data) override;
		void setFloat3(Shader* shader, const int32_t& valueID, const float* data) override;
		void setFloat4(Shader* shader, const int32_t& valueID, const float* data) override;


		void setFloat1(Shader* shader, const int32_t& valueID, const float& data) override;
		void setFloat2(Shader* shader, const int32_t& valueID, const float2& data) override;
		void setFloat3(Shader* shader, const int32_t& valueID, const float3& data) override;
		void setFloat4(Shader* shader, const int32_t& valueID, const float4& data) override;

		void setInt1(Shader* shader, const int32_t& valueID, const int32_t& data) override;
		void setInt1(Shader* shader, const int32_t& valueID, int32_t* data) override;
		void setInt2(Shader* shader, const int32_t& valueID, int32_t* data) override;
		void setInt3(Shader* shader, const int32_t& valueID, int32_t* data) override;
		void setInt4(Shader* shader, const int32_t& valueID, int32_t* data) override;

		void setMat3(Shader* shader, const int32_t& valueID, const float* data) override;
		void setMat3(Shader* shader, const int32_t& valueID, const float3x3& mat3) override;
		void setMat4(Shader* shader, const int32_t& valueID, const float* data) override;
		void setMat4(Shader* shader, const int32_t& valueID, const float4x4& mat4) override;
		void setMat4(Shader* shader, const int32_t& valueID, float4x4 data[], int32_t count) override;

		void setGlobalTexture2D(Shader* shader, const int32_t& valueID, Texture2D* data) override;
		void setTexture2D(Shader* shader, const int32_t& valueID, Texture2D* data) override;
		void setNullTexture2D(Shader* shader, const int32_t& valueID) override;
		void setGlobalTextureCube(Shader* shader, const int32_t& valueID, TextureCube* data) override;
		void setTextureCube(Shader* shader, const int32_t& valueID, TextureCube* data) override;

	public:
		void cmdUpdateTexture2D(Texture2D* tex2d) override;

	private:
		void initContext();

	private:
		GLFWwindow* mWindow;
		int32_t mSavePolygonMode;
	};
}
