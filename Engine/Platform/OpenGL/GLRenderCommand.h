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
		GLRenderCMD_Vertex(Vertex* vertex);
		virtual ~GLRenderCMD_Vertex();

		void run(Shader* shader) override;
	};

	class GLRenderCMD_Shadow : public RenderCommand
	{
	public:
		GLRenderCMD_Shadow(Vertex* vertex, Transform* transform);
		virtual ~GLRenderCMD_Shadow();

		void run(Shader* shader) override;
	};

	class GLRenderCMD_Skybox : public RenderCommand
	{
	public:
		GLRenderCMD_Skybox(Vertex* vertex, TextureCube* cube, float lod = 0, bool isHdr = false, float exposure = 1);
		virtual ~GLRenderCMD_Skybox();

		void run(Shader* shader) override;
	private:
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

		void run(Shader* shader) override;
	};


	class GLRenderCMD_HDRToCube : public RenderCommand
	{
	public:
		GLRenderCMD_HDRToCube(Vertex* vertex
			, Texture2D* texHDR
			, TextureCube* skybox);
		virtual ~GLRenderCMD_HDRToCube();

		void run(Shader* shader) override;

	private:
		Texture2D* mTexHDR;
		TextureCube* mSkybox;
	};

	class GLRenderCMD_EnvMakeIrradiance : public RenderCommand
	{
	public:
		GLRenderCMD_EnvMakeIrradiance(Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance);
		virtual ~GLRenderCMD_EnvMakeIrradiance();

		void run(Shader* shader) override;

	private:
		TextureCube* mSkyboxMap;
		TextureCube* mIrradianceMap;
	};

	class GLRenderCMD_EnvMakePrefilter : public RenderCommand
	{
	public:
		GLRenderCMD_EnvMakePrefilter(Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32 mipMaxLevel
			, uint32 mipWidth
			, uint32 mipHeight
			, float resolution);
		virtual ~GLRenderCMD_EnvMakePrefilter();

		void run(Shader* shader) override;

	private:
		TextureCube* mSkyboxMap;
		TextureCube* mPrefilterMap;
		uint32 mMipMaxLevel;
		uint32 mMipWidth;
		uint32 mMipHeight;
		float mResolution;
	};
}
