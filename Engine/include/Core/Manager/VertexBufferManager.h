#pragma once
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
#include "../Head/CppHead.h"
#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	class Vertex;
	class VertexBuffer;
	class IndexBuffer;
	class MeshData;
	class UniformBuffer;
	class UniformBufferLayout;

	class TINY_API VertexBufferManager
	{
		VertexBufferManager() = delete;
		~VertexBufferManager() = delete;

	public:
		static Vertex* create(const std::string& name);
		static Vertex* create(MeshData* mesh);

		static void add(MeshData* meshData);
		static void add(Vertex* vertex);

		static MeshData* findMeshData(const std::string& name);
		static Vertex* findVertex(const std::string& name);

		static void init();

	private:
		static std::unordered_map<std::string_view, Vertex*> mVertexUMap;
		static std::unordered_map<std::string_view, MeshData*> mMeshDataUMap;
		static std::vector<Vertex*> mVertexAry;


	public:
		static std::tuple<bool, std::shared_ptr<UniformBufferLayout>> createUniformBufferLayout(const std::string& name);
		static std::shared_ptr<UniformBufferLayout> getUniformBufferLayout(const std::string& name);

	private:
		static std::unordered_map<std::string, std::shared_ptr<UniformBufferLayout>> mUniformBufferMap;
	};
}
