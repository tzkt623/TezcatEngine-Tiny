#pragma once

#include "../Renderer/RenderObject.h"
#include "../Head/Context.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class VertexGroup;
	class Shader;
	class Material;
	class MeshData;
	class TINY_API MeshRenderer : public ComponentT<MeshRenderer>, public IRenderMesh
	{
	public:
		MeshRenderer();
		MeshRenderer(MeshData* meshData);
		MeshRenderer(VertexGroup* vg);
		virtual ~MeshRenderer();

		RenderObjectType getRenderObjectType() final { return RenderObjectType::MeshRenderer; }
		VertexGroup* getVertexGroup() const { return mVertexGroup; }

		Material* getMaterial() const override { return mMainMaterial; }
		void setMaterial(Material* val)
		{
			mMainMaterial = val;
		}
		void setMesh(const std::string& meshName);

		void beginRender() override;
		void endRender() override;
		void submit(Shader* shader) override;
		void submitModelMatrix(Shader* shader) override;
		void sendToRenderPass(const RenderPassType& passType) override;

		DrawModeWrapper& getDrawMode() final { return mDrawMode; }
		void setDrawMode(DrawMode val) { mDrawMode = ContextMap::DrawModeArray[(int)val]; }

		bool hasIndex() const { return mHasIndex; }
		int getVertexCount() const final;
		int getIndexCount() const final;

	protected:
		void onEnable() override;
		void onDisable() override;
		void onStart() override;

	private:
		bool mIsCastShadow;
		bool mHasIndex;
		DrawModeWrapper mDrawMode;
		VertexGroup* mVertexGroup;
		Material* mMainMaterial;
	};
}
