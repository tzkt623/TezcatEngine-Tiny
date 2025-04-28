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
#include "../Base/TinyObject.h"
#include "../Tool/Tool.h"
#include "../Head/ConfigHead.h"
#include "../Head/TinyCpp.h"
#include "../Renderer/Texture.h"

namespace tezcat::Tiny
{
	class ShadowCaster;
	class Shader;

	/*
	* @brief 阴影投射器管理器
	*/
	class TINY_API ShadowCasterManager
	{
		friend class ShadowCaster;
		static uint32_t add(ShadowCaster* caster);
		static void recycle(ShadowCaster* caster);

	public:
		static std::list<TinyWeakRef<ShadowCaster>>& getAllCaster() { return mCasterAry; }
		static void init();
		static void submit(Shader* mShader);

	public:
		static Texture2D* getOrCreateDirectionalShadowMap();
		static TextureCube* getOrCreatePointShadowMap();
		static Texture2D* getOrCreateSpotShadowMap();

		static void recycleDirectionalShadowMap(Texture2D* tex);
		static void recyclePointShadowMap(TextureCube* tex);
		static void recycleSpotShadowMap(Texture2D* tex);

	private:
		template<class TTexture>
		struct TexturePool
		{
			std::stack<TTexture*> freeArray;
			uint32_t width;
			uint32_t height;
			uint32_t length;

			std::tuple<FlagCreate, TTexture*> createOrGet()
			{
				TTexture* t = nullptr;
				if (!freeArray.empty())
				{
					t = freeArray.top();
					freeArray.pop();
					return { FlagCreate::Existed, t };
				}

				t = TTexture::create();
				t->setSizeWHL(width, height, length);
				t->saveObject();
				return { FlagCreate::Succeeded, t };
			}

			void recycle(TTexture* tex)
			{
				freeArray.push(tex);
			}

			void setSizeWHL(uint32_t width, uint32_t height, uint32_t length)
			{
				this->width = width;
				this->height = height;
				this->length = length;
			}

			~TexturePool()
			{
				while (!freeArray.empty())
				{
					freeArray.top()->deleteObject();
					freeArray.pop();
				}
			}
		};

	private:
		static bool mAdded;
		static std::list<TinyWeakRef<ShadowCaster>> mCasterAry;
		static std::queue<uint32_t> mFreeIDs;
		static Shader* mShader;
		static TexturePool<Texture2D> mDirectionalLightShadowMaps;

	};
}
