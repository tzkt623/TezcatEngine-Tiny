#pragma once

#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Manager/VertexManager.h"
#include "Core/Head/GLMHead.h"

using namespace tezcat::Tiny::Core;
namespace tezcat::Tiny::GL
{
	class TINY_API GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer();
		virtual ~GLVertexBuffer();

		void init(MeshData* meshData) override;

	private:
		uint32_t* mVBOArray;
		int mVBOSize;
	};


	class TINY_API GLVertex : public Vertex
	{
	public:
		GLVertex();
		virtual ~GLVertex();

		void init(MeshData* meshData) override;
		void bind() override;
		void unbind() override;
	};


	class TINY_API GLVertexCreator : public VertexCreator
	{
	public:
		GLVertexCreator();
		virtual ~GLVertexCreator();
		VertexBuffer* createVertexBuffer() override;
		Vertex* createVertex() override;
	};
}
