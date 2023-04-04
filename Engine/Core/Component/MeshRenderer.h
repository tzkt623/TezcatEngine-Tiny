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
	class TINY_API MeshRenderer : public ComponentT<MeshRenderer>, public IRenderObject
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
			this->addMaterialConfig();
		}
		void setMesh(const std::string& meshName);

		void submit(Shader* shader) override;
		void sendToRenderPass() override;

		void onEnable() override;
		void onDisable() override;
		void onStart() override;

		int getVertexCount() const final;
		int getIndexCount() const final;
		DrawModeWrapper& getDrawMode() final { return mDrawMode; }
		void setDrawMode(DrawMode val) { mDrawMode = ContextMap::DrawModeArray[(int)val]; }

		bool hasIndex() const { return mHasIndex; }


	private:
		void addMaterialConfig();

	private:
		DrawModeWrapper mDrawMode;

		bool mHasIndex;
		VertexGroup* mVertexGroup;
		Material* mMainMaterial;
	};
}
