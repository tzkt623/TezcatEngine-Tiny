#include "VertexGroup.h"
#include "../Data/MeshData.h"
#include "../Manager/VertexGroupManager.h"
#include "Utility/Utility.h"
#include "VertexBuffer.h"
#include "BaseGraphics.h"


namespace tezcat::Tiny::Core
{
	VertexGroup::VertexGroup()
		: mUID(0)
		, mIndexCount(0)
		, mVertexBuffer(nullptr)
		, mVertexCount(0)
		, mName("##ErrorVAO")
	{

	}

	VertexGroup::~VertexGroup()
	{
		delete mVertexBuffer;
	}
}

