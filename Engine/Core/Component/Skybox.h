#pragma once

#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny::Core
{
	class VertexGroup;
	class Skybox : public ComponentT<Skybox>, IRenderObject
	{
	public:
		Skybox();
		virtual ~Skybox();

	public:
		RenderObjectType getRenderObjectType() final { return RenderObjectType::Skybox; }
		DrawModeWrapper& getDrawMode() override;
		int getVertexCount() const override;
		Material* getMaterial() const override;
		void sendToRenderPass() override;
		void submit(Shader* shader) override;
		void setMaterial(Material* material) { mMaterial = material; }


	protected:
		void onStart() override;
		void onEnable() override;

	private:
		DrawModeWrapper mDrawMode;
		Material* mMaterial;
		VertexGroup* mVertexGroup;
	};
}


