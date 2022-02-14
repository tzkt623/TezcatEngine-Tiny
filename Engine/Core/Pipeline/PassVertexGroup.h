#pragma once
#include "../Head/CppHead.h"

namespace tezcat::Tiny::Core
{
	class MeshRenderer;
	class Shader;
	class VertexGroup;
	class PassVertexGroup
	{
	public:
		PassVertexGroup(VertexGroup* vao);
		~PassVertexGroup();
		void addMeshRenderer(MeshRenderer* renderAgent);
		void render(Shader* shader);
		bool empty() const { return m_MeshRendererList.empty(); }
	private:
		VertexGroup* m_DelegateVertexGroup;
		std::list<MeshRenderer*> m_MeshRendererList;
	};
}