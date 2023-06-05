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

	Vertex::Vertex(const std::string& name)
		: mName(name)
	{

	}

	Vertex::~Vertex()
	{
		mVertexBuffers.clear();

		if (mIndexBuffer)
		{
			mIndexBuffer->subRef();
		}
		delete mChildren;
	}

	void Vertex::init(MeshData* meshData)
	{
		mDrawModeWrapper = ContextMap::DrawModeArray[(uint32_t)meshData->mDrawMode];
		mVertexCount = meshData->mVertices.size();
		mName = meshData->getName();

		for (auto& position : meshData->mLayoutPositions)
		{
			auto [size, data] = meshData->getVertexData(position);
			auto buffer = VertexBuffer::create();
			buffer->init(size, data);
			buffer->setLayoutData(position, VertexLayout::getLayoutType(position));
			this->setVertexBuffer(buffer);
		}

		auto [size, data] = meshData->getIndexData();
		if (size > 0)
		{
			mIndexCount = meshData->mIndices.size();
			auto index = IndexBuffer::create();
			index->init(size, data);
			this->setIndexBuffer(index);
		}
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

	void Vertex::generate()
	{
		Graphics::getInstance()->cmdCreateVertex(this);
	}

}

