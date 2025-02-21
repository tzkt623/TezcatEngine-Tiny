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

