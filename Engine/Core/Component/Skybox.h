#pragma once

#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny::Core
{
	class Vertex;
	class Skybox : public ComponentT<Skybox>, public IRenderMesh
	{
	public:
		Skybox();
		virtual ~Skybox();

	public:
		RenderObjectType getRenderObjectType() final { return RenderObjectType::Skybox; }
		DrawModeWrapper& getDrawMode() override;
		int getVertexCount() const override;
		Material* getMaterial() const override;

		void sendToRenderPass(const RenderPassType& passType) override;
		void submit(Shader* shader) override;

		void setMaterial(Material* material) { mMaterial = material; }
		void beginRender() override;
		void endRender() override;

	protected:
		void onStart() override;
		void onEnable() override;

	private:
		DrawModeWrapper mDrawMode;
		Material* mMaterial;
		Vertex* mVertexGroup;
	};
}


