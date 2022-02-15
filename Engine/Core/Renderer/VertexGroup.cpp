#include "VertexGroup.h"
#include "../Data/MeshData.h"
#include "../Manager/VertexGroupManager.h"
#include "Utility/Tools.h"
#include "VertexBuffer.h"
#include "BaseGraphics.h"


namespace tezcat::Tiny::Core
{
	VertexGroup::VertexGroup()
		: m_UID(0)
		, m_IndexCount(0)
		, m_VertexBuffer(nullptr)
		, m_VertexCount(0)
		, m_Name("##ErrorVAO")
	{

	}

	VertexGroup::~VertexGroup()
	{
		delete m_VertexBuffer;
	}
}

