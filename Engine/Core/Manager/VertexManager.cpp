#include "VertexManager.h"
#include "../Renderer/Vertex.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/BaseGraphics.h"
#include "../Data/MeshData.h"

namespace tezcat::Tiny::Core
{
	VertexManager::VertexManager()
	{
		VertexMgr::attach(this);
	}

	VertexManager::~VertexManager()
	{

	}

	Vertex* VertexManager::getVertex(const std::string& name)
	{
		return mVertexUMap[name];
	}

	void VertexManager::addMeshData(MeshData* meshData)
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

	MeshData* VertexManager::findMeshData(const std::string& name)
	{
		auto it = mMeshDataUMap.find(name);
		if (it != mMeshDataUMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	VertexBuffer* VertexManager::createVertexBuffer(MeshData* meshData)
	{
		auto buffer = mCreator->createVertexBuffer();
		buffer->init(meshData);
		return buffer;
	}

	Vertex* VertexManager::createVertex(MeshData* meshData)
	{
		auto vertex = mCreator->createVertex();
		vertex->init(meshData);

		this->buildChild(vertex, meshData);

		mVertexUMap.emplace(meshData->getName(), vertex);
		return vertex;
	}

	void VertexManager::buildChild(Vertex* vertexParent, MeshData* meshParent)
	{
		if (meshParent->hasChildren())
		{
			for (auto cmesh : meshParent->getChildren())
			{
				auto cvertex = mCreator->createVertex();
				cvertex->init(cmesh);
				vertexParent->addChild(cvertex);
				this->buildChild(cvertex, cmesh);
			}
		}
	}

}
