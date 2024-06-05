#include "Vertex.h"
#include "../Data/MeshData.h"
#include "../Manager/VertexBufferManager.h"
#include "../Tool/Tool.h"

#include "VertexBuffer.h"
#include "BaseGraphics.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Vertex, TinyObject)
	Vertex::Vertex()
		: Vertex("##ErrorVAO")
	{

	}

	Vertex::Vertex(std::string name)
		: mName(std::move(name))
		, mChildren(nullptr)
		, mIndexBuffer(nullptr)
	{

	}

	Vertex::~Vertex()
	{

	}

	void Vertex::setMesh(MeshData* meshData)
	{
		mDrawModeWrapper = ContextMap::DrawModeArray[(uint32)meshData->mDrawMode];
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

	void Vertex::setMesh(const std::string& name, const uint32& vertexCount, const DrawMode& drawMode)
	{
		mName = name;
		mDrawModeWrapper = ContextMap::DrawModeArray[(uint32)drawMode];
		mVertexCount = vertexCount;
	}

	void Vertex::addChild(Vertex* vertex)
	{
		if (mChildren == nullptr)
		{
			mChildren = new std::vector<Vertex*>();
		}

		mChildren->emplace_back(vertex);
	}

	void Vertex::setVertexBuffer(VertexBuffer* buffer)
	{
		buffer->saveObject();
		mVertexBuffers.push_back(buffer);
	}

	void Vertex::setIndexBuffer(IndexBuffer* buffer)
	{
		mIndexBuffer = buffer;
		mIndexBuffer->saveObject();
	}

	void Vertex::generate()
	{
		Graphics::getInstance()->cmdCreateVertex(this);
	}

	void Vertex::onClose()
	{
		Graphics::getInstance()->cmdDeleteVertex(mVertexID);

		for (auto buffer : mVertexBuffers)
		{
			buffer->deleteObject();
		}
		mVertexBuffers.clear();

		if (mIndexBuffer)
		{
			mIndexBuffer->deleteObject();
		}

		if (mChildren)
		{
			for (auto child : *mChildren)
			{
				child->deleteObject();
			}

			delete mChildren;
		}
	}
}

