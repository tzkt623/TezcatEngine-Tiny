#pragma once

#include "../Head/RenderConfig.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/RenderObject.h"


namespace tezcat::Tiny
{
	class Vertex;
	class Shader;
	class Material;
	class MeshData;
	class RenderAgent;
	class TINY_API MeshRenderer : public ComponentT<MeshRenderer>, public IRenderMesh
	{
		MeshRenderer();
		TINY_Factory(MeshRenderer);
		TINY_RTTI_H(MeshRenderer);
	public:
		virtual ~MeshRenderer();
		Vertex* getVertex() const { return mVertex; }

		void sendToQueue(BaseGraphics* graphics, const RenderPhase& phase, RenderQueue* queue) override;
		void setCastShadow(bool val) { mIsCastShadow = val; }

	protected:
		void onEnable() override;
		void onDisable() override;
		void onStart() override;

	private:
		bool mIsCastShadow;
	};
}
