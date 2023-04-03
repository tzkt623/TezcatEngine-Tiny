#pragma once
#include "../Head/CppHead.h"
#include "../Head/Context.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class MeshRenderer;
	class Shader;
	class VertexGroup;
	class TINY_API PassVertexGroup
	{
	public:
		PassVertexGroup(VertexGroup* vao);
		~PassVertexGroup();
		void addRenderObject(MeshRenderer* renderAgent);
		void render(Shader* shader);
		void render(Shader* shader, DrawModeWrapper& drawMode);
		bool empty() const { return mMeshRendererList.empty(); }

	private:
		VertexGroup* mDelegateVertexGroup;
		std::list<MeshRenderer*> mMeshRendererList;
	};
}