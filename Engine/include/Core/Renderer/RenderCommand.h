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
#include "../Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class BaseGraphics;
	class Shader;
	class FrameBuffer;
	class UniformBuffer;
	class Texture2D;
	class TextureCube;
	class TextureRender2D;
	class Vertex;
	class VertexBuffer;
	class IndexBuffer;
	class UniformBuffer;
	class Material;
	class Transform;
	class PipelinePass;
	
	enum class CMDLife
	{
		Once,
		Countor,
		Continue,
		Death
	};

	enum class CommandID : uint32_t
	{
		None = 0,
		DrawMesh,
	};

	enum class RenderCommandType
	{
		Normal,
		Build,
		Draw
	};

	class RenderCommand
	{
	public:
		//union
		//{
		//	uint64 mOrderID;
		//
		//	struct
		//	{
		//		uint64 shaderID : 16;
		//		uint64 priority : 32;
		//		uint64 userID : 16;
		//	} mBase;
		//
		//	struct
		//	{
		//		uint64 shaderID : 16;
		//		uint64 distanceFromCamera : 32;
		//		uint64 userID : 16;
		//	} mTransparent;
		//};

	protected:
		RenderCommand();

	public:
		virtual ~RenderCommand();
		virtual void execute(PipelinePass* pass = nullptr, Shader* shader = nullptr) = 0;
		virtual RenderCommandType getType() const { return RenderCommandType::Normal; }
	};

	class RenderCMD_Lambda : public RenderCommand
	{
	public:
		RenderCMD_Lambda(std::function<void(PipelinePass*, Shader*)> function);

		void execute(PipelinePass* pass, Shader* shader) override;

	private:
		std::function<void(PipelinePass*, Shader*)> mFuncion;
	};

#pragma region Build

	class RenderCommadBuild : public RenderCommand
	{
	public:
		using RenderCommand::RenderCommand;
		RenderCommandType getType() const final { return RenderCommandType::Build; }
	};

	class RenderCMD_CreateVertex : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateVertex(Vertex* vertex);
		virtual ~RenderCMD_CreateVertex();

		void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
	};

	class RenderCMD_DeleteVertex : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteVertex(Vertex* buffer);
		virtual ~RenderCMD_DeleteVertex();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		uint32_t mID;
	};

	class RenderCMD_DeleteVertexBuffer : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteVertexBuffer(VertexBuffer* buffer);
		virtual ~RenderCMD_DeleteVertexBuffer();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		uint32_t mID;
	};

	class RenderCMD_DeleteIndexBuffer : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteIndexBuffer(IndexBuffer* buffer);
		virtual ~RenderCMD_DeleteIndexBuffer();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		uint32_t mID;
	};

	class RenderCMD_DeleteUniformBuffer : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteUniformBuffer(UniformBuffer* buffer);
		virtual ~RenderCMD_DeleteUniformBuffer();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		uint32_t mID;
	};

	class RenderCMD_CreateShader : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateShader(Shader* shader);
		virtual ~RenderCMD_CreateShader();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Shader* mShader;
	};

	class RenderCMD_DeleteShader : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteShader(Shader* shader);
		virtual ~RenderCMD_DeleteShader();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		uint32_t mID;
	};

	class RenderCMD_RebuildShader : public RenderCommadBuild
	{
	public:
		RenderCMD_RebuildShader(Shader* shader);
		virtual ~RenderCMD_RebuildShader();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Shader* mShader;
	};

	class RenderCMD_CreateFrameBuffer : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateFrameBuffer(FrameBuffer* buffer);
		virtual ~RenderCMD_CreateFrameBuffer();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		FrameBuffer* mBuffer;
	};

	class RenderCMD_DeleteFrameBuffer : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteFrameBuffer(FrameBuffer* frameBuffer);
		virtual ~RenderCMD_DeleteFrameBuffer();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		uint32_t mID;
	};

	class RenderCMD_CreateUniformBuffer : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateUniformBuffer(UniformBuffer* buffer, int32_t index);
		virtual ~RenderCMD_CreateUniformBuffer();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		UniformBuffer* mBuffer;
		int32_t mIndex;
	};

	class RenderCMD_CreateTexture2D : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateTexture2D(Texture2D* tex);
		virtual ~RenderCMD_CreateTexture2D();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Texture2D* mTex;
	};

	class RenderCMD_DeleteTexture2D : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteTexture2D(Texture2D* tex);
		virtual ~RenderCMD_DeleteTexture2D();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		uint32_t mID;
	};

	class RenderCMD_CreateTextureRender2D : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateTextureRender2D(TextureRender2D* tex);
		virtual ~RenderCMD_CreateTextureRender2D();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		TextureRender2D* mTex;
	};

	class RenderCMD_DeleteTextureRender2D : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteTextureRender2D(TextureRender2D* tex);
		virtual ~RenderCMD_DeleteTextureRender2D();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		uint32_t mID;
	};

	class RenderCMD_CreateTextureCube : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateTextureCube(TextureCube* tex);
		virtual ~RenderCMD_CreateTextureCube();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		TextureCube* mTex;
	};

	class RenderCMD_DeleteTextureCube : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteTextureCube(TextureCube* tex);
		virtual ~RenderCMD_DeleteTextureCube();

		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		uint32_t mID;
	};

	class RenderCMD_ReadObjectID : public RenderCommadBuild
	{
	public:
		RenderCMD_ReadObjectID(int x, int y, FrameBuffer* fb);
		virtual ~RenderCMD_ReadObjectID();
		void execute(PipelinePass* pass, Shader* shader) override;
	private:
		int mX;
		int mY;
		FrameBuffer* mFrameBuffer;
	};

	class RenderCMD_ClearTexture2D : public RenderCommadBuild
	{
	public:
		RenderCMD_ClearTexture2D(Texture2D* tex);
		virtual ~RenderCMD_ClearTexture2D();
		void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Texture2D* mTexture;
	};
#pragma endregion Build


#pragma region Draw
	class RenderCommadDraw : public RenderCommand
	{
	public:
		using RenderCommand::RenderCommand;
		RenderCommandType getType() const final { return RenderCommandType::Draw; }
	};

	class RenderCMD_DrawVertex : public RenderCommadDraw
	{
		/*此命令可以合并,优化DrawCall*/
	public:
		RenderCMD_DrawVertex(Vertex* vertex);
		virtual ~RenderCMD_DrawVertex();
		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
	};

	class RenderCMD_DrawMeshWithMaterial : public RenderCommadDraw
	{
		/*此命令可以合并,优化DrawCall*/
	public:
		RenderCMD_DrawMeshWithMaterial(Vertex* vertex, Transform* transform, Material* material);
		virtual ~RenderCMD_DrawMeshWithMaterial();
		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
		Transform* mTransform;
		Material* mMaterial;
	};

	class RenderCMD_DrawMeshWithOutMaterial : public RenderCommadDraw
	{
		/*此命令可以合并,优化DrawCall*/
	public:
		RenderCMD_DrawMeshWithOutMaterial(Vertex* vertex, Transform* transform);
		virtual ~RenderCMD_DrawMeshWithOutMaterial();
		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
		Transform* mTransform;
	};

	class RenderCMD_DrawShadow : public RenderCommadDraw
	{
		/*此命令可以合并,优化DrawCall*/
	public:
		RenderCMD_DrawShadow(Vertex* vertex, Transform* transform);
		virtual ~RenderCMD_DrawShadow();
		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
		Transform* mTransform;
	};


	class RenderCMD_DrawID : public RenderCommadDraw
	{
		/*此命令可以合并,优化DrawCall*/
	public:
		RenderCMD_DrawID(Vertex* vertex, Transform* transform);
		virtual ~RenderCMD_DrawID();
		virtual void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
		Transform* mTransform;
	};


	class RenderCMD_DrawSkybox : public RenderCommadDraw
	{
	public:
		RenderCMD_DrawSkybox(Vertex* vertex, TextureCube* cube, float lod = 0, bool isHdr = false, float exposure = 1);
		virtual ~RenderCMD_DrawSkybox();

		void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
		TextureCube* mSkybox;
		float mLod;
		bool mIsHdr;
		float mExposure;
	};

	class RenderCMD_MakeHDR2Cube : public RenderCommadDraw
	{
	public:
		RenderCMD_MakeHDR2Cube(Vertex* vertex
			, Texture2D* texHDR
			, TextureCube* skybox
			, std::array<int32_t, 2> viewSize);
		virtual ~RenderCMD_MakeHDR2Cube();

		void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
		Texture2D* mTexHDR;
		TextureCube* mSkybox;
		std::array<int32_t, 2> mViewSize;
	};

	class RenderCMD_MakeEnvIrradiance : public RenderCommadDraw
	{
	public:
		RenderCMD_MakeEnvIrradiance(Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance
			, std::array<int32_t, 2> viewSize);
		virtual ~RenderCMD_MakeEnvIrradiance();

		void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
		TextureCube* mIrradiance;
		TextureCube* mSkybox;
		std::array<int32_t, 2> mViewSize;
	};

	class RenderCMD_MakeEnvPrefilter : public RenderCommadDraw
	{
	public:
		RenderCMD_MakeEnvPrefilter(Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32_t mipMaxLevel
			, uint32_t mipWidth
			, uint32_t mipHeight
			, int32_t resolution);
		virtual ~RenderCMD_MakeEnvPrefilter();

		void execute(PipelinePass* pass, Shader* shader) override;

	private:
		Vertex* mVertex;
		TextureCube* mSkyboxMap;
		TextureCube* mPrefilterMap;
		uint32_t mMipMaxLevel;
		uint32_t mMipWidth;
		uint32_t mMipHeight;
		int32_t mResolution;
	};

#pragma endregion Draw
}
