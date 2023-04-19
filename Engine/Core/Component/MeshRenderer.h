#pragma once

#include "../Renderer/RenderObject.h"
#include "../Head/Context.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Vertex;
	class Shader;
	class Material;
	class MeshData;
	class TINY_API MeshRenderer : public ComponentT<MeshRenderer>, public IRenderMesh
	{
	public:
		MeshRenderer();
		MeshRenderer(MeshData* meshData);
		MeshRenderer(Vertex* vg);
		virtual ~MeshRenderer();

		RenderObjectType getRenderObjectType() final { return RenderObjectType::MeshRenderer; }
		Vertex* getVertex() const { return mVertex; }

		Material* getMaterial() const override { return mMainMaterial; }
		void setMaterial(Material* val)
		{
			mMainMaterial = val;
		}
		void setMesh(const std::string& meshName);
		void setMesh(MeshData* meshData);

		void beginRender() override;
		void endRender() override;
		void submit(Shader* shader) override;
		void submitModelMatrix(Shader* shader) override;
		void sendToRenderPass(const RenderPassType& passType) override;

		DrawModeWrapper& getDrawMode() final { return mDrawMode; }
		void setDrawMode(DrawMode val) { mDrawMode = ContextMap::DrawModeArray[(int)val]; }

		int getVertexCount() const final;
		int getIndexCount() const final;

		void setCastShadow(bool val) { mIsCastShadow = val; }

	protected:
		void onEnable() override;
		void onDisable() override;
		void onStart() override;

	private:
		bool mIsCastShadow;
		DrawModeWrapper mDrawMode;
		Vertex* mVertex;
		Material* mMainMaterial;
	};
}
