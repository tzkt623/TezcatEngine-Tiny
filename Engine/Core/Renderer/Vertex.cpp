#include "Vertex.h"
#include "../Data/MeshData.h"
#include "../Manager/VertexBufferManager.h"
#include "../Tool/Tool.h"

#include "VertexBuffer.h"
#include "BaseGraphics.h"


namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Vertex);
	Vertex::Vertex()
		: mName("##ErrorVAO")
	{

	}

	Vertex::~Vertex()
	{
		if (mIndexBuffer)
		{
			mIndexBuffer->subRef();
		}
		delete mChildren;
	}

	void Vertex::init(MeshData* meshData)
	{
		mDrawModeWrapper = ContextMap::DrawModeArray[(uint32_t)meshData->mDrawMode];
		mName = meshData->getName();
	}

	void Vertex::init(const std::string& name, const size_t& vertexCount, const DrawMode& drawMode)
	{
		mName = name;
		mDrawModeWrapper = ContextMap::DrawModeArray[(uint32_t)drawMode];
		mVertexCount = (uint32_t)vertexCount;
	}

	void Vertex::addChild(Vertex* vertex)
	{
		if (mChildren == nullptr)
		{
			mChildren = new TinyVector<Vertex*>();
		}

		mChildren->emplace_back(vertex);
	}

	void Vertex::setVertexBuffer(VertexBuffer* buffer)
	{
		mVertexBuffers.push_back(buffer);
	}

	void Vertex::setIndexBuffer(IndexBuffer* buffer)
	{
		mIndexBuffer = buffer;
		mIndexBuffer->addRef();
	}
}

