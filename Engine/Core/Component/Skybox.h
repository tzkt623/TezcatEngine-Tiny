#pragma once

#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny
{
	class Vertex;
	class RenderAgent;
	class Skybox : public ComponentT<Skybox>, public IRenderMesh
	{
		Skybox();
		TINY_RTTI_H(Skybox)
		TINY_Factory(Skybox)
	public:
		
		virtual ~Skybox();

	public:
		RenderObjectType getRenderObjectType() final { return RenderObjectType::Skybox; }
		DrawModeWrapper& getDrawMode() override;
		int getVertexCount() const override;
		Material* getMaterial() const override;

		void sendToRenderPass(const RenderPassType& passType) override;
		void submit(Shader* shader) override;

		void setMaterial(Material* material);
		void beginRender() override;
		void endRender() override;

	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;

	private:
		DrawModeWrapper mDrawMode;
		Material* mMaterial;
		Vertex* mVertex;
		RenderAgent* mRenderAgent;
	};
}


