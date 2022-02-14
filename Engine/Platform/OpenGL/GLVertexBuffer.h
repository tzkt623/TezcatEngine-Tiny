#pragma once

#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/VertexGroup.h"
#include "Core/Head/GLMHead.h"

namespace tezcat::Tiny::Core
{
	class TINY_API GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(MeshData* meshData);
		virtual ~GLVertexBuffer();

	protected:
		void init(MeshData* meshData) override;

	private:
		uint32_t* m_VBOArray;
		int m_VBOSize;
	};


	class TINY_API GLVertexGroup : public VertexGroup
	{
	public:
		GLVertexGroup(MeshData* meshData);
		virtual ~GLVertexGroup();

		void initWithMesh(MeshData* mesh) override;
		void bind() override;
	};
}