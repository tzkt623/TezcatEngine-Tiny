#pragma once

#include "../Head/CppHead.h"
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
		void removeCaster(ShadowCaster* caster);
		std::vector<ShadowCaster*>& getAllCaster() { return mCasterAry; }
		Shader* getShader();

	private:
		std::vector<ShadowCaster*> mCasterAry;
		std::deque<uint32_t> mFreeIDs;
		Shader* mShader;
		Texture2D* mShadowTexture;

	public:
		void calculate(BaseGraphics* graphics);
		void submit(Shader* mShader);
	};


	using ShadowCasterMgr = SG<ShadowCasterManager>;
}
