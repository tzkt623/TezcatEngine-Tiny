#pragma once
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class Shader;

	class TINY_API TextureAgent
	{
		virtual void bind(Shader* shader) = 0;
	};

	class TINY_API Texture2DAgent : public TextureAgent
	{

	};

	class TINY_API Texture3DAgent : public TextureAgent
	{

	};

	class TINY_API TextureCubeAgent : public TextureAgent
	{

	};

	class TINY_API TextureRender2DAgent : public TextureAgent
	{

	};

	class TINY_API GraphicsAgentFactoryImp
	{
	public:
		virtual Texture2DAgent* createTexture2DAgent() = 0;
		virtual Texture3DAgent* createTexture3DAgent() = 0;
		virtual TextureCubeAgent* createTextureCubeAgent() = 0;
		virtual TextureRender2DAgent* createTextureRender2DAgent() = 0;
	};

	class TINY_API GraphicsAgentFactory
	{

	public:
		static Texture2DAgent* createTexture2DAgent()
		{
			return mImp->createTexture2DAgent();
		}

		static Texture3DAgent* createTexture3DAgent()
		{
			return mImp->createTexture3DAgent();
		}

		static TextureCubeAgent* createTextureCubeAgent()
		{
			return mImp->createTextureCubeAgent();
		}

		static TextureRender2DAgent* createTextureRender2DAgent()
		{
			return mImp->createTextureRender2DAgent();
		}

	private:
		static GraphicsAgentFactoryImp* mImp;
	};
}

