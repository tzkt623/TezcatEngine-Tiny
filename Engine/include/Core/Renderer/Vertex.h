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
#include "VertexConfig.h"

#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/RenderConfig.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class MeshData;
	class VertexBuffer;
	class IndexBuffer;

	/// <summary>
	/// 对应gl的VAO
	/// 
	/// </summary>
	class TINY_API Vertex : public TinyObject
	{
		Vertex();
		Vertex(const std::string& name);
		TINY_OBJECT_H(Vertex, TinyObject)

	protected:
		virtual void init() override {}
		std::string getMemoryInfo() override;

	public:
		virtual ~Vertex();
		const uint32_t& getVertexCount() const { return mVertexCount; }
		const uint32_t &getIndexCount() const { return mIndexCount; }
		DrawModeWrapper& getDrawMode() { return mDrawModeWrapper; }

		const uint32_t &getVertexID() const { return mVertexID; }
		void apply(uint32_t id) { mVertexID = id; }
		void generate();

		void setVertexBuffer(VertexBuffer* buffer);
		void setIndexBuffer(IndexBuffer* buffer);
		void setVertexCount(uint32_t size) { mVertexCount = size; }
		void setIndexCount(uint32_t size) { mIndexCount = size; }

		const std::vector<VertexBuffer*>& getVertexBuffers() const
		{
			return mVertexBuffers;
		}

		IndexBuffer* getIndexBuffer() const
		{
			return mIndexBuffer;
		}

	public:
		void setMesh(MeshData* mesh);
		void setMesh(const std::string& name, const uint32_t& vertexCount, const DrawMode& drawMode);

	protected:
		virtual void onClose() override;

	protected:
		uint32_t mVertexID;

		uint32_t mVertexCount;
		uint32_t mIndexCount;

		DrawModeWrapper mDrawModeWrapper;
		std::vector<VertexBuffer*> mVertexBuffers;
		IndexBuffer* mIndexBuffer;
	};
}
