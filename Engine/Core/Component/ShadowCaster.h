#pragma once

#include "../Head/RenderConfig.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny
{
	class FrameBuffer;
	class ExtraQueue;
	class Texture2D;
	class TINY_API ShadowCaster : public ComponentT<ShadowCaster>, public IRenderObserver
	{
		ShadowCaster();
		TINY_Factory(ShadowCaster);
		TINY_RTTI_H(ShadowCaster);
	public:
		virtual ~ShadowCaster();

		void submit(BaseGraphics* graphcis, Shader* shader) override;
		void submitViewMatrix(BaseGraphics* graphcis, Shader* shader) override;

		void setShadowMap(int width, int height, const std::string& shaderName);

		uint32_t getCasterID() { return mCasterID; }
		Texture2D* getShadwowTexutre() { return mShadwowTexutre; }


	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;
		void onDestroy() override;

	private:
		uint32_t mCasterID;
		Texture2D* mShadwowTexutre;
	};
}
