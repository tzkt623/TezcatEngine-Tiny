#pragma once

#include "../Base/TinyObject.h"
#include "../Tool/Tool.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class ShadowCaster;
	class Shader;
	class Texture2D;
	class BaseGraphics;

	/*
	* @brief 阴影投射器管理器
	*/
	class TINY_API ShadowCasterManager
	{
	public:
		ShadowCasterManager();

		uint32_t addCaster(ShadowCaster* caster);
		std::list<TinyWeakRef<ShadowCaster>>& getAllCaster() { return mCasterAry; }
		Shader* getShader();
	public:
		void calculate(BaseGraphics* graphics);
		void submit(BaseGraphics* graphics, Shader* mShader);

	private:
		std::list<TinyWeakRef<ShadowCaster>> mCasterAry;
		std::deque<uint32_t> mFreeIDs;
		Shader* mShader;
		Texture2D* mShadowTexture;
	};


	using ShadowCasterMgr = SG<ShadowCasterManager>;
}
