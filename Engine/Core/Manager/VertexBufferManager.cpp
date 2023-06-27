#include "VertexBufferManager.h"
#include "../Renderer/Vertex.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/BaseGraphics.h"
#include "../Data/MeshData.h"

#include "ThirdParty/Hash/city.h"

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

	void VertexBufferManager::add(MeshData* meshData)
	{
		if (meshData->getName().empty())
		{
			return;
		}

		auto result = mMeshDataUMap.try_emplace(meshData->getName(), nullptr);
		if (result.second)
		{
			meshData->addRef();
			result.first->second = meshData;
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

	void VertexBufferManager::add(Vertex* vertex)
	{
		auto result = mVertexUMap.try_emplace(vertex->getName(), vertex);
		if (!result.second)
		{
			result.first->second->subRef();
			result.first->second = vertex;
		}
		vertex->addRef();
	}

	Vertex* VertexBufferManager::create(const std::string& name)
	{
		if (name.empty())
		{
			return nullptr;
		}

		auto hash_id = CityHash64(name.data(), name.size());

		if (!mMeshDataUMap.contains(name))
		{
			return nullptr;
		}

		auto result = mVertexUMap.try_emplace(name, nullptr);
		if (result.second)
		{
			auto vertex = Vertex::create();
			vertex->init(mMeshDataUMap[name]);
			vertex->generate();
			vertex->addRef();
			result.first->second = vertex;
		}

		return result.first->second;
	}

	Vertex* VertexBufferManager::create(MeshData* mesh)
	{
		return nullptr;
	}
}
