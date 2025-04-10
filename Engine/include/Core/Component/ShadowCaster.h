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

	class TINY_API ShadowCaster : public ComponentAutoID<ShadowCaster>
	{
		ShadowCaster();
		TINY_OBJECT_H(ShadowCaster, ComponentAutoID<ShadowCaster>)
		TINY_RENDER_OBSERVER_FUNCTION(mShadowObserver)
	public:
		virtual ~ShadowCaster();
		void setShadowMap(int width, int height, const std::string& shaderName);

		uint32_t getUID() const { return mUID; }
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
		uint32_t mUID;
		Texture2D* mShadowTexture;
		ShadowObserver* mShadowObserver;
		ReplacedPipelinePass* mPipePass;
		FrameBuffer* mFrameBuffer;
	};
}
