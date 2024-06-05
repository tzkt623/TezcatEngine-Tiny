#pragma once

#include "../Head/ConfigHead.h"
#include "../Component/Component.h"
#include "../Renderer/RenderObserver.h"

namespace tezcat::Tiny
{
	class FrameBuffer;
	class Texture2D;
	class ReplacedPipelinePass;
	class Shader;
	class RenderObserver;

	class TINY_API ShadowCaster : public ComponentT<ShadowCaster>
	{
		ShadowCaster();
		TINY_OBJECT_H(ShadowCaster, ComponentT<ShadowCaster>)
		TINY_RENDER_OBSERVER_FUNCTION(mShadowObserver)
	public:
		virtual ~ShadowCaster();
		void setShadowMap(int width, int height, const std::string& shaderName);

		uint32 getUID() const { return mUID; }
		Texture2D* getShadowTexutre() { return mShadowTexture; }

		void submit(Shader* shader);

	protected:
		virtual void init() override;

	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;
		void onDestroy() override;

	private:
		uint32 mUID;
		Texture2D* mShadowTexture;
		RenderObserver* mShadowObserver;
		ReplacedPipelinePass* mPipePass;
		FrameBuffer* mFrameBuffer;
	};
}
