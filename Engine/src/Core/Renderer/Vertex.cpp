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

#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/RenderCommand.h"

#include "Core/Data/MeshData.h"
#include "Core/Manager/VertexBufferManager.h"
#include "Core/Tool/Tool.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Vertex, TinyObject);
	Vertex::Vertex()
		: Vertex("##ErrorVAO")
	{

	}

	Vertex::Vertex(const std::string& name)
		: mVertexID(0)
		, mVertexCount(0)
		, mIndexCount(0)
		, mIndexBuffer(nullptr)
	{
		mEngineName = name;
	}

	Vertex::~Vertex()
	{

	}

	void Vertex::setMesh(MeshData* meshData)
	{
		mDrawModeWrapper = ContextMap::DrawModeArray[(uint32_t)meshData->mDrawMode];
		mVertexCount = (uint32_t)meshData->mVertices.size();
		mEngineName = meshData->getName();

		mVertexBuffers.reserve(meshData->mLayoutPositions.size());
		for (auto& position : meshData->mLayoutPositions)
		{
			auto [size, data] = meshData->getVertexData(position);
			auto buffer = VertexBuffer::create();
			buffer->init(size, data);
			buffer->setLayoutData(position);
			this->setVertexBuffer(buffer);
		}

		auto [size, data] = meshData->getIndexData();
		if (size > 0)
		{
			mIndexCount = (uint32_t)meshData->mIndices->size();
			auto index = IndexBuffer::create();
			index->init(size, data);
			this->setIndexBuffer(index);
		}
	}

	void Vertex::setMesh(const std::string& name, const uint32_t& vertexCount, const DrawMode& drawMode)
	{
		mEngineName = name;
		mDrawModeWrapper = ContextMap::DrawModeArray[(uint32_t)drawMode];
		mVertexCount = vertexCount;
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
		Graphics::getInstance()->addCommand(new RenderCMD_CreateVertex(this));
	}

	void Vertex::onClose()
	{
		Graphics::getInstance()->addCommand<RenderCMD_DeleteVertex>(this);

		for (auto buffer : mVertexBuffers)
		{
			buffer->deleteObject();
		}
		mVertexBuffers.clear();

		if (mIndexBuffer)
		{
			mIndexBuffer->deleteObject();
		}
	}

	std::string Vertex::getMemoryInfo()
	{
		return TINY_OBJECT_MEMORY_INFO();
	}
}

