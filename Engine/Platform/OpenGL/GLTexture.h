#pragma once
#include "Core/Renderer/Texture.h"
#include "GLHead.h"
#include "Core/Head/ConfigHead.h"


namespace tezcat::Tiny::Core
{
	class TINY_API GLTexture2D : public Texture
	{
	public:
		GLTexture2D();
		virtual ~GLTexture2D();

		inline TextureType getTextureType() const override { return TextureType::Texture2D; }

		void createTexture(Image* image) override;
		void createTexture(const std::string& filePath) override;
	};
}