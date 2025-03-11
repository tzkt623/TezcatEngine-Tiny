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

	private:
		static bool mAdded;
		static std::list<TinyWeakRef<ShadowCaster>> mCasterAry;
		static std::queue<uint32_t> mFreeIDs;
		static Shader* mShader;
	};
}
