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
	class TINY_API MeshRenderer : public RenderObejct
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

		int getVertexCount() const;
		int getIndexCount() const;

		inline DrawModeWrapper& getDrawMode() { return m_DrawMode; }
		void setDrawMode(DrawMode val) { m_DrawMode = ContextMap::DrawModeArray[(int)val]; }

		inline bool hasIndex() const { return mHasIndex; }

		bool extraLight();

	private:
		void addMaterialConfig();

	private:
		DrawModeWrapper m_DrawMode;

		bool mHasIndex;
		VertexGroup* mVertexGroup;
		Material* mMainMaterial;
	};
}
