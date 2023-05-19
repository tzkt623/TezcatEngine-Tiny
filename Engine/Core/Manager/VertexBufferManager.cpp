#include "VertexBufferManager.h"
#include "../Renderer/Vertex.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/BaseGraphics.h"
#include "../Data/MeshData.h"

namespace tezcat::Tiny
{
	VertexBufferManager::VertexBufferManager()
	{
		VertexBufMgr::attach(this);
	}

	VertexBufferManager::~VertexBufferManager()
	{
		for (auto& pair : mVertexUMap)
		{
			pair.second->subRef();
		}
		mVertexUMap.clear();
	}

	Vertex* VertexBufferManager::getVertex(const std::string& name)
	{
		return mVertexUMap[name];
	}

	Vertex* VertexBufferManager::getVertex(MeshData* meshData)
	{
		auto vertex = this->findVertex(meshData->getName());

		if (vertex == nullptr)
		{
			vertex = this->createVertexAndCached(meshData);
		}

		return vertex;
	}

	void VertexBufferManager::addMeshData(MeshData* meshData)
	{
		if (meshData->getName().empty())
		{
			return;
		}

		auto result = mMeshDataUMap.try_emplace(meshData->getName(), nullptr);
		if (result.second)
		{
			result.first->second = meshData;
		}
		else
		{
			if (result.first->second != meshData)
			{
				delete meshData;
			}
		}
	}

	Vertex* VertexBufferManager::findVertex(const std::string& name)
	{
		auto it = mVertexUMap.find(name);
		if (it != mVertexUMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	MeshData* VertexBufferManager::findMeshData(const std::string& name)
	{
		auto it = mMeshDataUMap.find(name);
		if (it != mMeshDataUMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	VertexBuffer* VertexBufferManager::createVertexBuffer(const void* data, const size_t& length)
	{
		auto buffer = mCreator->createVertexBuffer();
		buffer->bind();
		buffer->init(data, length);
		return buffer;
	}

	VertexBuffer* VertexBufferManager::createVertexBuffer(const size_t& length)
	{
		auto buffer = mCreator->createVertexBuffer();
		buffer->bind();
		buffer->init(length);
		return buffer;
	}

	IndexBuffer* VertexBufferManager::createIndexBuffer(const void* data, const size_t& length)
	{
		auto buffer = mCreator->createIndexBuffer();
		buffer->bind();
		buffer->init(data, length);
		return buffer;
	}

	Vertex* VertexBufferManager::createVertexAndCached(MeshData* meshData)
	{
		auto result = mVertexUMap.try_emplace(meshData->getName(), nullptr);
		if (result.second)
		{
			auto vertex = mCreator->createVertex();
			this->buildVertex(vertex, meshData);
			this->buildChild(vertex, meshData);

			vertex->addRef();
			result.first->second = vertex;
		}

		return result.first->second;
	}

	Vertex* VertexBufferManager::createVertex(MeshData* meshData)
	{
		auto vertex = mCreator->createVertex();
		this->buildVertex(vertex, meshData);
		this->buildChild(vertex, meshData);
		return vertex;
	}

	void VertexBufferManager::buildChild(Vertex* vertexParent, MeshData* meshParent)
	{
		if (meshParent->hasChildren())
		{
			for (auto cmesh : meshParent->getChildren())
			{
				auto cvertex = mCreator->createVertex();
				this->buildVertex(cvertex, cmesh);
				vertexParent->addChild(cvertex);
				this->buildChild(cvertex, cmesh);
			}
		}
	}

	void VertexBufferManager::buildVertex(Vertex* vertex, MeshData* meshData)
	{
		vertex->bind();

		vertex->init(meshData->getName(), meshData->vertices.size(), meshData->drawMode);
		for (auto p : meshData->layoutPositions)
		{
			auto [length, data] = meshData->getVertexData(p);
			auto vbuffer = this->createVertexBuffer(data, length);
			vbuffer->setLayoutData(p, VertexLayout::getLayoutType(p));
			vertex->setVertexBuffer(vbuffer);
		}

		vertex->setVertexCount(meshData->vertices.size());

		if (!meshData->indices.empty())
		{
			auto ibuffer = this->createIndexBuffer(meshData->indices.data(), meshData->indexSize());
			vertex->setIndexBuffer(ibuffer);
			vertex->setIndexCount(meshData->indices.size());
		}

		vertex->unbind();
	}

}
