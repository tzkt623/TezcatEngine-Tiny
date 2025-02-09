#pragma once

#include "../Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class BaseGraphics;
	class Shader;
	class FrameBuffer;
	class Texture2D;
	class TextureCube;
	class TextureRender2D;
	class Vertex;
	class Material;
	class Transform;
	
	enum class CMDLife
	{
		Once,
		Countor,
		Continue,
		Death
	};

	enum class CommandID : uint32
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
		union
		{
			uint64 mOrderID;

			struct
			{
				uint64 shaderID : 16;
				uint64 priority : 32;
				uint64 userID : 16;
			} mBase;

			struct
			{
				uint64 shaderID : 16;
				uint64 distanceFromCamera : 32;
				uint64 userID : 16;
			} mTransparent;
		};

	protected:
		RenderCommand();

	public:
		virtual ~RenderCommand();
		virtual void execute(Shader* shader) = 0;
		virtual RenderCommandType getType() const { return RenderCommandType::Normal; }
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

		void execute(Shader* shader) override;

	private:
		Vertex* mVertex;
	};

	class RenderCMD_CreateShader : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateShader(Shader* shader);
		virtual ~RenderCMD_CreateShader();

		virtual void execute(Shader* shader) override;

	private:
		Shader* mShader;
	};

	class RenderCMD_DeleteShader : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteShader(Shader* shader);
		virtual ~RenderCMD_DeleteShader();

		virtual void execute(Shader* shader) override;

	private:
		Shader* mShader;
	};

	class RenderCMD_RebuildShader : public RenderCommadBuild
	{
	public:
		RenderCMD_RebuildShader(Shader* shader);
		virtual ~RenderCMD_RebuildShader();

		virtual void execute(Shader* shader) override;

	private:
		Shader* mShader;
	};

	class RenderCMD_CreateFrameBuffer : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateFrameBuffer(FrameBuffer* buffer);
		virtual ~RenderCMD_CreateFrameBuffer();

		virtual void execute(Shader* shader) override;

	private:
		FrameBuffer* mBuffer;
	};

	class RenderCMD_CreateTexture2D : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateTexture2D(Texture2D* tex);
		virtual ~RenderCMD_CreateTexture2D();

		virtual void execute(Shader* shader) override;

	private:
		Texture2D* mTex;
	};

	class RenderCMD_DeleteTexture2D : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteTexture2D(Texture2D* tex);
		virtual ~RenderCMD_DeleteTexture2D();

		virtual void execute(Shader* shader) override;

	private:
		uint32 mID;
	};

	class RenderCMD_CreateTextureRender2D : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateTextureRender2D(TextureRender2D* tex);
		virtual ~RenderCMD_CreateTextureRender2D();

		virtual void execute(Shader* shader) override;

	private:
		TextureRender2D* mTex;
	};

	class RenderCMD_DeleteTextureRender2D : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteTextureRender2D(TextureRender2D* tex);
		virtual ~RenderCMD_DeleteTextureRender2D();

		virtual void execute(Shader* shader) override;

	private:
		uint32 mID;
	};

	class RenderCMD_CreateTextureCube : public RenderCommadBuild
	{
	public:
		RenderCMD_CreateTextureCube(TextureCube* tex);
		virtual ~RenderCMD_CreateTextureCube();

		virtual void execute(Shader* shader) override;

	private:
		TextureCube* mTex;
	};

	class RenderCMD_DeleteTextureCube : public RenderCommadBuild
	{
	public:
		RenderCMD_DeleteTextureCube(TextureCube* tex);
		virtual ~RenderCMD_DeleteTextureCube();

		virtual void execute(Shader* shader) override;

	private:
		uint32 mID;
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
		virtual void execute(Shader* shader) override;

	private:
		Vertex* mVertex;
	};

	class RenderCMD_DrawMesh : public RenderCommadDraw
	{
		/*此命令可以合并,优化DrawCall*/
	public:
		RenderCMD_DrawMesh(Vertex* vertex, Transform* transform, Material* material);
		virtual ~RenderCMD_DrawMesh();
		virtual void execute(Shader* shader) override;

	private:
		Vertex* mVertex;
		Transform* mTransform;
		Material* mMaterial;
	};

	class RenderCMD_DrawShadow : public RenderCommadDraw
	{
		/*此命令可以合并,优化DrawCall*/
	public:
		RenderCMD_DrawShadow(Vertex* vertex, Transform* transform);
		virtual ~RenderCMD_DrawShadow();
		virtual void execute(Shader* shader) override;

	private:
		Vertex* mVertex;
		Transform* mTransform;
	};


	class RenderCMD_DrawSkybox : public RenderCommadDraw
	{
	public:
		RenderCMD_DrawSkybox(Vertex* vertex, TextureCube* cube, float lod = 0, bool isHdr = false, float exposure = 1);
		virtual ~RenderCMD_DrawSkybox();

		void execute(Shader* shader) override;

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
			, TextureCube* skybox);
		virtual ~RenderCMD_MakeHDR2Cube();

		void execute(Shader* shader) override;

	private:
		Vertex* mVertex;
		Texture2D* mTexHDR;
		TextureCube* mSkybox;
	};

	class RenderCMD_MakeEnvIrradiance : public RenderCommadDraw
	{
	public:
		RenderCMD_MakeEnvIrradiance(Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance);
		virtual ~RenderCMD_MakeEnvIrradiance();

		void execute(Shader* shader) override;

	private:
		Vertex* mVertex;
		TextureCube* mIrradiance;
		TextureCube* mSkybox;
	};

	class RenderCMD_MakeEnvPrefilter : public RenderCommadDraw
	{
	public:
		RenderCMD_MakeEnvPrefilter(Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32 mipMaxLevel
			, uint32 mipWidth
			, uint32 mipHeight
			, int32 resolution);
		virtual ~RenderCMD_MakeEnvPrefilter();

		void execute(Shader* shader) override;

	private:
		Vertex* mVertex;
		TextureCube* mSkyboxMap;
		TextureCube* mPrefilterMap;
		uint32 mMipMaxLevel;
		uint32 mMipWidth;
		uint32 mMipHeight;
		int32 mResolution;
	};

#pragma endregion Draw
}
