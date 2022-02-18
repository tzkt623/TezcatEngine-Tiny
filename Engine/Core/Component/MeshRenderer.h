#pragma once
#include "RenderObject.h"
#include "../Head/Context.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class VertexGroup;
	class Shader;
	class Material;
	class MeshData;
	class TINY_API MeshRenderer : public RenderObject<MeshRenderer>
	{
	public:

	public:
		MeshRenderer();
		MeshRenderer(MeshData* meshData);
		MeshRenderer(VertexGroup* vg);
		virtual ~MeshRenderer();

		RenderObjectType getRenderObjectType() final { return RenderObjectType::MeshRenderer; }
		VertexGroup* getVertexGroup() const { return m_VertexGroup; }

		inline Material* getMaterial() const { return m_MainMaterial; }
		void setMaterial(Material* val)
		{
			m_MainMaterial = val;
			this->addMaterialConfig();
		}
		void setMesh(const std::string& meshName);

		void sendDataToGPU() override;
		void submit(Shader* shader) override;

		void onEnable() override;
		void onStart() override;

		int getVertexCount() const;
		int getIndexCount() const;

		inline DrawModeWrapper& getDrawMode() { return m_DrawMode; }
		void setDrawMode(DrawMode val) { m_DrawMode = ContextMap::DrawModeArray[(int)val]; }

		inline bool hasIndex() const { return m_HasIndex; }

	private:
		void addMaterialConfig();

	private:
		DrawModeWrapper m_DrawMode;

		bool m_HasIndex;
		VertexGroup* m_VertexGroup;
		Material* m_MainMaterial;
	};
}