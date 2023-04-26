#pragma once
#include "../Head/CppHead.h"
#include "../Head/Context.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class MeshRenderer;
	class Shader;
	class Vertex;
	class TINY_API PassVertexGroup
	{
	public:
		PassVertexGroup(Vertex* vao);
		~PassVertexGroup();
		void addRenderObject(MeshRenderer* renderAgent);
		bool empty() const { return mMeshRendererList.empty(); }

	private:
		Vertex* mDelegateVertexGroup;
		std::list<MeshRenderer*> mMeshRendererList;
	};
}
