#pragma once

#include "../Base/TinyObject.h"
#include "../Tool/Tool.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class ShadowCaster;
	class Shader;
	class BaseGraphics;

	/*
	* @brief 阴影投射器管理器
	*/
	class TINY_API ShadowCasterManager
	{
		friend class ShadowCaster;
		static uint32 add(ShadowCaster* caster);
		static void recycle(ShadowCaster* caster);
	public:
		static std::list<TinyWeakRef<ShadowCaster>>& getAllCaster() { return mCasterAry; }
		static void init();
		static void submit(BaseGraphics* graphics, Shader* mShader);

	private:
		static bool mAdded;
		static std::list<TinyWeakRef<ShadowCaster>> mCasterAry;
		static std::queue<uint32> mFreeIDs;
		static Shader* mShader;
	};
}
