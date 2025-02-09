/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Manager/VertexBufferManager.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Data/MeshData.h"

#include "ThirdParty/Hash/city.h"

namespace tezcat::Tiny
{
	std::vector<Vertex*> VertexBufferManager::mVertexAry;
	std::unordered_map<std::string_view, MeshData*> VertexBufferManager::mMeshDataUMap;
	std::unordered_map<std::string_view, Vertex*> VertexBufferManager::mVertexUMap;

	void VertexBufferManager::add(MeshData* meshData)
	{
		if (meshData->getName().empty())
		{
			return;
		}

		auto result = mMeshDataUMap.try_emplace(meshData->getName(), nullptr);
		if (result.second)
		{
			meshData->saveObject();
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
		auto result = mVertexUMap.try_emplace(vertex->getName(), nullptr);
		if (result.second)
		{
			result.first->second = vertex;
			vertex->saveObject();
		}
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
			vertex->setMesh(mMeshDataUMap[name]);
			vertex->generate();
			vertex->saveObject();
			result.first->second = vertex;
		}

		return result.first->second;
	}

	Vertex* VertexBufferManager::create(MeshData* mesh)
	{
		return nullptr;
	}
}
