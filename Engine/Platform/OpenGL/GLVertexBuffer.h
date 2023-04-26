#pragma once

#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Manager/VertexManager.h"
#include "Core/Head/GLMHead.h"

using namespace tezcat::Tiny;
namespace tezcat::Tiny::GL
{
	class TINY_API GLVertexBuffer : public VertexBuffer
	{
		GLVertexBuffer();
	public:
		TINY_Factory(GLVertexBuffer)

	public:
		virtual ~GLVertexBuffer();

		void init(const size_t& dataLenght) override;
		void init(const void* data, const size_t& dataLenght) override;
		void setData(const void* data, size_t size) override;


		void bind() override;
		void unbind() override;


	};

	class TINY_API GLIndexBuffer : public IndexBuffer
	{
		GLIndexBuffer();
	public:
		TINY_Factory(GLIndexBuffer)
	public:
		virtual ~GLIndexBuffer();

		void init(const void* data, const size_t& dataLenght) override;
		void bind() override;
		void unbind() override;

	private:

	};


	class TINY_API GLVertex : public Vertex
	{
		GLVertex();
	public:
		TINY_Factory(GLVertex)
	public:
		virtual ~GLVertex();

		void bind() override;
		void unbind() override;

		void init(const std::string& name, const size_t& vertexCount, const DrawMode& drawMode) override;
		void setVertexBuffer(VertexBuffer* buffer) override;
		void setIndexBuffer(IndexBuffer* buffer) override;
	};


	class TINY_API GLVertexCreator : public VertexCreator
	{
	public:
		GLVertexCreator();
		virtual ~GLVertexCreator();

		VertexBuffer* createVertexBuffer() override;
		Vertex* createVertex() override;
		IndexBuffer* createIndexBuffer() override;
	};
}
