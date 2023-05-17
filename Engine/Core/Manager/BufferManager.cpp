#include "BufferManager.h"
#include "../Renderer/Vertex.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/BaseGraphics.h"
#include "../Data/MeshData.h"

namespace tezcat::Tiny
{
	BufferManager::BufferManager()
	{
		BufferMgr::attach(this);
	}

	BufferManager::~BufferManager()
	{
		for (auto& pair : mVertexUMap)
		{
			pair.second->subRef();
		}
		mVertexUMap.clear();
	}

	Vertex* BufferManager::getVertex(const std::string& name)
	{
		return mVertexUMap[name];
	}

	Vertex* BufferManager::getVertex(MeshData* meshData)
	{
		auto vertex = this->findVertex(meshData->getName());

		if (vertex == nullptr)
		{
			vertex = this->createVertexAndCached(meshData);
		}

		return vertex;
	}

	void BufferManager::addMeshData(MeshData* meshData)
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

	Vertex* BufferManager::findVertex(const std::string& name)
	{
		auto it = mVertexUMap.find(name);
		if (it != mVertexUMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	MeshData* BufferManager::findMeshData(const std::string& name)
	{
		auto it = mMeshDataUMap.find(name);
		if (it != mMeshDataUMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	VertexBuffer* BufferManager::createVertexBuffer(const void* data, const size_t& length)
	{
		auto buffer = mCreator->createVertexBuffer();
		buffer->bind();
		buffer->init(data, length);
		return buffer;
	}

	VertexBuffer* BufferManager::createVertexBuffer(const size_t& length)
	{
		auto buffer = mCreator->createVertexBuffer();
		buffer->bind();
		buffer->init(length);
		return buffer;
	}

	IndexBuffer* BufferManager::createIndexBuffer(const void* data, const size_t& length)
	{
		auto buffer = mCreator->createIndexBuffer();
		buffer->bind();
		buffer->init(data, length);
		return buffer;
	}

	Vertex* BufferManager::createVertexAndCached(MeshData* meshData)
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

	Vertex* BufferManager::createVertex(MeshData* meshData)
	{
		auto vertex = mCreator->createVertex();
		this->buildVertex(vertex, meshData);
		this->buildChild(vertex, meshData);
		return vertex;
	}

	void BufferManager::buildChild(Vertex* vertexParent, MeshData* meshParent)
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

	void BufferManager::buildVertex(Vertex* vertex, MeshData* meshData)
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
