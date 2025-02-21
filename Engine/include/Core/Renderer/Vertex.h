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
		Vertex(std::string name);
		TINY_OBJECT_H(Vertex, TinyObject)

	protected:
		virtual void init() override {}

	public:
		virtual ~Vertex();
		void setName(const std::string& val) { mName = val; }
		std::string getName() const { return mName; }
		const uint32 &getVertexCount() const { return mVertexCount; }
		const uint32 &getIndexCount() const { return mIndexCount; }
		DrawModeWrapper& getDrawMode() { return mDrawModeWrapper; }

		const uint32 &getVertexID() const { return mVertexID; }
		void apply(uint32 id) { mVertexID = id; }
		void generate();

		void setVertexBuffer(VertexBuffer* buffer);
		void setIndexBuffer(IndexBuffer* buffer);
		void setVertexCount(uint32 size) { mVertexCount = size; }
		void setIndexCount(uint32 size) { mIndexCount = size; }

		void addChild(Vertex* vertex);

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
		void setMesh(const std::string& name, const uint32& vertexCount, const DrawMode& drawMode);

	protected:
		virtual void onClose() override;

	protected:
		std::string mName;
		uint32 mVertexID = 0;

		uint32 mVertexCount = 0;
		uint32 mIndexCount = 0;

		DrawModeWrapper mDrawModeWrapper;
		std::vector<VertexBuffer*> mVertexBuffers;
		IndexBuffer* mIndexBuffer;
		std::vector<Vertex*>* mChildren;
	};
}
