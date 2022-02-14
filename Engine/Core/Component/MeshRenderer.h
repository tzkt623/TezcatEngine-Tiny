#pragma once
#include "RenderObject.h"
#include "../Head/GLHead.h"
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

		RenderObjectType getRenderObjectType() final { return RenderObjectType::Mesh; }
		void submit(Shader* shader) override;
		VertexGroup* getVertexGroup() const { return m_VertexGroup; }

		Material* getMaterial() const { return m_MainMaterial; }
		void setMaterial(Material* val) { m_MainMaterial = val; }
		void setMesh(const std::string& meshName);

		void sendDataToGPU() override;

		void onEnable() override;
		void onStart() override;

		int getVertexCount() const;
		int getIndexCount() const;

		inline DrawMode getDrawMode() const { return m_DrawMode; }
		inline void setDrawMode(DrawMode val) { m_DrawMode = val; }

		inline bool hasIndex() const { return m_HasIndex; }

	private:
		DrawMode m_DrawMode;

		bool m_HasIndex;
		VertexGroup* m_VertexGroup;
		Material* m_MainMaterial;
	};
}