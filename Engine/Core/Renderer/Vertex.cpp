#include "Vertex.h"
#include "../Data/MeshData.h"
#include "../Manager/VertexManager.h"
#include "Utility/Utility.h"
#include "VertexBuffer.h"
#include "BaseGraphics.h"


namespace tezcat::Tiny::Core
{
	Vertex::Vertex()
		: mUID(0)
		, mIndexCount(0)
		, mVertexBuffer(nullptr)
		, mVertexCount(0)
		, mName("##ErrorVAO")
		, mChildren(nullptr)
	{

	}

	Vertex::~Vertex()
	{
		if (mChildren != nullptr)
		{
			for (auto c : *mChildren)
			{
				delete c;
			}
		}

		delete mChildren;
		delete mVertexBuffer;
	}

	void Vertex::addChild(Vertex* vertex)
	{
		if (mChildren == nullptr)
		{
			mChildren = new std::vector<Vertex*>();
		}

		mChildren->push_back(vertex);
	}

}

