#pragma once

#include "Core/Renderer/RenderCommand.h"

namespace tezcat::Tiny
{
	class Transform;
	class Vertex;
	class Material;
	class Texture2D;
	class TextureCube;
	class FrameBuffer;
}

namespace tezcat::Tiny::GL
{

	class GLRenderCMD_Vertex : public RenderCommand
	{
	public:
		GLRenderCMD_Vertex(Shader* shader, Vertex* vertex);
		virtual ~GLRenderCMD_Vertex();

		void run(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
	};

	class GLRenderCMD_Shadow : public RenderCommand
	{
	public:
		GLRenderCMD_Shadow(Vertex* vertex, Transform* transform);
		virtual ~GLRenderCMD_Shadow();

		void run(BaseGraphics* graphics, Shader* shader) override;

	private:
		Transform* mTransform;
		Vertex* mVertex;
	};

	class GLRenderCMD_Skybox : public RenderCommand
	{
	public:
		GLRenderCMD_Skybox(Vertex* vertex, TextureCube* cube, float lod = 0, bool isHdr = false, float exposure = 1);
		virtual ~GLRenderCMD_Skybox();

		void run(BaseGraphics* graphics, Shader* shader) override;
	private:
		Vertex* mVertex;
		TextureCube* mSkybox;
		float mLod;
		bool mIsHdr;
		float mExposure;
	};

	class GLRenderCMD_Mesh : public RenderCommand
	{
	public:
		GLRenderCMD_Mesh(Vertex* vertex, Transform* transform, Material* material);
		virtual ~GLRenderCMD_Mesh();

		void run(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
		Transform* mTransform;
		Material* mMaterial;
	};


	class GLRenderCMD_HDRToCube : public RenderCommand
	{
	public:
		GLRenderCMD_HDRToCube(Vertex* vertex
			, Texture2D* texHDR
			, TextureCube* skybox);
		virtual ~GLRenderCMD_HDRToCube();

		void run(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
		Texture2D* mTexHDR;
		TextureCube* mSkybox;
	};

	class GLRenderCMD_EnvMakeIrradiance : public RenderCommand
	{
	public:
		GLRenderCMD_EnvMakeIrradiance(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance);
		virtual ~GLRenderCMD_EnvMakeIrradiance();

		void run(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
		TextureCube* mSkyboxMap;
		TextureCube* mIrradianceMap;
	};

	class GLRenderCMD_EnvMakePrefilter : public RenderCommand
	{
	public:
		GLRenderCMD_EnvMakePrefilter(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32_t mipMaxLevel
			, uint32_t mipWidth
			, uint32_t mipHeight
			, float resolution);
		virtual ~GLRenderCMD_EnvMakePrefilter();

		void run(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
		TextureCube* mSkyboxMap;
		TextureCube* mPrefilterMap;
		uint32_t mMipMaxLevel;
		uint32_t mMipWidth;
		uint32_t mMipHeight;
		float mResolution;
	};
}
