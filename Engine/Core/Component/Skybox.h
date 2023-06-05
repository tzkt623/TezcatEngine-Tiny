#pragma once

#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny
{
	class Vertex;
	class RenderAgent;
	class Skybox : public ComponentT<Skybox>, public IRenderMesh
	{
		Skybox();
		TINY_RTTI_H(Skybox);
		TINY_Factory(Skybox);
	public:
		
		virtual ~Skybox();

		void sendToQueue(BaseGraphics* graphics, const RenderPhase& phase, RenderQueue* queue) override;

	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;
	};
}


