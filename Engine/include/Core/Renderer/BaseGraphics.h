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

#include "../Head/ConfigHead.h"
#include "../Head/GLMHead.h"
#include "../Renderer/RenderConfig.h"

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
	class BaseRenderObserver;
	class Camera;
	class Pipeline;
	class TextureRender2D;
	class FrameBuffer;
	class Vertex;
	class VertexBuffer;
	class IndexBuffer;
	class UniformBuffer;
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

		void setPolygonMode(PolygonMode mode);

	public:
		virtual void preRender() {}
		virtual void postRender() {}

	public:
		virtual void createVertex(Vertex* vertex) {}
		virtual void createShader(Shader* shader) {}
		virtual void createBuffer(VertexBuffer* buffer) {}
		virtual void createBuffer(IndexBuffer* buffer) {}
		virtual void createBuffer(FrameBuffer* buffer) {}
		virtual void createUniformBuffer(UniformBuffer* buffer, int32_t bindingIndex) {}
		virtual void createTexture(Texture2D* tex2D) {}
		virtual void createTexture(TextureCube* texCube) {}
		virtual void createTexture(TextureRender2D* render2D) {}

		virtual void createMipmapTexCube(TextureCube* texCube, int32_t minLevel, int32_t maxLevel) {}

	public:
		void setClearColor(float r, float g, float b, float a)
		{
			mClearColor.r = r;
			mClearColor.g = g;
			mClearColor.b = b;
			mClearColor.a = a;
		}

		void setClearColor(const float4& color)
		{
			mClearColor = color;
		}


		/*
		* 注意 所有删除方法均不能出现具体类
		* 因为必须在RenderCommad中进行
		*/
	public:
		virtual void deleteShader(const uint32_t& id) {}
		virtual void deleteVertex(const uint32_t& id) {}
		virtual void deleteVertexBuffer(const uint32_t& id) {}
		virtual void deleteIndexBuffer(const uint32_t& id) {}
		virtual void deleteUniformBuffer(const uint32_t& id) {}
		virtual void deleteTexture2D(const uint32_t& id) {}
		virtual void deleteTextureCube(const uint32_t& id) {}
		virtual void deleteRender2D(const uint32_t& id) {}
		virtual void deleteFrameBuffer(const uint32_t& id) {}

	public:
		virtual void makeHDR2Cube(Shader* shader
			, BaseRenderObserver* observer
			, Vertex* vertex
			, Texture2D* texHDR
			, TextureCube* skybox
			, std::array<int32_t, 2> viewSize) {}
		virtual void makeEnvIrradiance(Shader* shader
			, BaseRenderObserver* observer
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance
			, std::array<int32_t, 2> viewSize) {}
		virtual void makeEnvPrefilter(Shader* shader
			, BaseRenderObserver* observer
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32_t mipMaxLevel
			, uint32_t mipWidth
			, uint32_t mipHeight
			, int32_t resolution) {}

	public:
		virtual void setUniformBuffer(UniformBuffer* uniformBuffer) {}
		virtual void setFrameBufferColorTexture2D(Texture2D* tex, int32_t index) {}
		virtual void setFrameBufferColorTextureCube(TextureCube* tex, int32_t index, int32_t cubeFace) {}


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

		virtual void setUInt1(Shader* shader, const char* name, const uint32_t& data) = 0;
		virtual void setInt1(Shader* shader, const char* name, const int32_t& data) = 0;
		virtual void setInt1(Shader* shader, const char* name, int32_t* data) = 0;
		virtual void setInt2(Shader* shader, const char* name, int32_t* data) = 0;
		virtual void setInt3(Shader* shader, const char* name, int32_t* data) = 0;
		virtual void setInt4(Shader* shader, const char* name, int32_t* data) = 0;

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

		virtual void setInt1(Shader* shader, UniformID& uniform, const int32_t& data) = 0;
		virtual void setInt1(Shader* shader, UniformID& uniform, int32_t* data) = 0;
		virtual void setInt2(Shader* shader, UniformID& uniform, int32_t* data) = 0;
		virtual void setInt3(Shader* shader, UniformID& uniform, int32_t* data) = 0;
		virtual void setInt4(Shader* shader, UniformID& uniform, int32_t* data) = 0;

		virtual void setMat3(Shader* shader, UniformID& uniform, float* data) = 0;
		virtual void setMat3(Shader* shader, UniformID& uniform, const float3x3& mat3) = 0;
		virtual void setMat4(Shader* shader, UniformID& uniform, const float* data) = 0;
		virtual void setMat4(Shader* shader, UniformID& uniform, const float4x4& mat4) = 0;
		virtual void setMat4(Shader* shader, UniformID& uniform, float4x4 data[], int32_t count) = 0;

		virtual void setGlobalTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) = 0;
		virtual void setTexture2D(Shader* shader, UniformID& uniform, Texture2D* data) = 0;

		virtual void setGlobalTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) = 0;
		virtual void setTextureCube(Shader* shader, UniformID& uniform, TextureCube* data) = 0;


		//-------------------------------------------------------
		//
		//	User Interface
		//
	public:
		virtual void setFloat1(Shader* shader, const int32_t& valueID, const float* data) = 0;
		virtual void setFloat2(Shader* shader, const int32_t& valueID, const float* data) = 0;
		virtual void setFloat3(Shader* shader, const int32_t& valueID, const float* data) = 0;
		virtual void setFloat4(Shader* shader, const int32_t& valueID, const float* data) = 0;

		virtual void setFloat1(Shader* shader, const int32_t& valueID, const float& data) = 0;
		virtual void setFloat2(Shader* shader, const int32_t& valueID, const float2& data) = 0;
		virtual void setFloat3(Shader* shader, const int32_t& valueID, const float3& data) = 0;
		virtual void setFloat4(Shader* shader, const int32_t& valueID, const float4& data) = 0;


		virtual void setInt1(Shader* shader, const int32_t& valueID, const int32_t& data) = 0;
		virtual void setInt1(Shader* shader, const int32_t& valueID, int32_t* data) = 0;
		virtual void setInt2(Shader* shader, const int32_t& valueID, int32_t* data) = 0;
		virtual void setInt3(Shader* shader, const int32_t& valueID, int32_t* data) = 0;
		virtual void setInt4(Shader* shader, const int32_t& valueID, int32_t* data) = 0;

		virtual void setMat3(Shader* shader, const int32_t& valueID, const float* data) = 0;
		virtual void setMat3(Shader* shader, const int32_t& valueID, const float3x3& mat3) = 0;
		virtual void setMat4(Shader* shader, const int32_t& valueID, const float* data) = 0;
		virtual void setMat4(Shader* shader, const int32_t& valueID, const float4x4& mat4) = 0;
		virtual void setMat4(Shader* shader, const int32_t& valueID, float4x4 data[], int32_t count) = 0;

		virtual void setGlobalTexture2D(Shader* shader, const int32_t& valueID, Texture2D* data) = 0;
		virtual void setTexture2D(Shader* shader, const int32_t& valueID, Texture2D* data) = 0;
		virtual void setNullTexture2D(Shader* shader, const int32_t& valueID) = 0;

		virtual void setGlobalTextureCube(Shader* shader, const int32_t& valueID, TextureCube* data) = 0;
		virtual void setTextureCube(Shader* shader, const int32_t& valueID, TextureCube* data) = 0;

		//----------------------------------------------------
		//
		//	Set Data
		//
	public:
		virtual void clear(const ClearOption& option) = 0;
		virtual void setViewport(const ViewportInfo& info) = 0;
		virtual void setPassState(Shader* shader) = 0;
		virtual void bind(Shader* shader) = 0;
		void bind(FrameBuffer* frameBuffer);
		void unbind(FrameBuffer* frameBuffer);
		virtual void bind(UniformBuffer* uniformBuffer) = 0;
		virtual void bind(Texture2D* texture) = 0;
		virtual void readPixel(int32_t x, int32_t y) = 0;
		virtual void readObjectID(int32_t x, int32_t y, int32_t& id) = 0;

	protected:
		virtual void bindImpl(FrameBuffer* frameBuffer) = 0;

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
		std::queue<RenderCommadBuild*> mBuildCommandQueue;
		std::stack<FrameBuffer*> mFrameBufferStack;

	protected:
		PolygonModeWrapper mPolygonMode;
		float4 mClearColor;
	};

	using Graphics = SG<BaseGraphics>;
}

