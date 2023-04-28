#include "GLVertexBuffer.h"
#include "GLHead.h"
#include "Core/Data/MeshData.h"

namespace tezcat::Tiny::GL
{
	GLVertexBuffer::GLVertexBuffer()
	{
		glGenBuffers(1, &mBufferID);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &mBufferID);
	}

	void GLVertexBuffer::init(const void* data, const size_t& dataLenght)
	{
		glBufferData(GL_ARRAY_BUFFER, dataLenght, data, GL_STATIC_DRAW);
	}

	void GLVertexBuffer::init(const size_t& dataLenght)
	{
		glBufferData(GL_ARRAY_BUFFER, dataLenght, nullptr, GL_DYNAMIC_DRAW);
	}

	/*
	void GLVertexBuffer::init(MeshData* meshData)
	{
		mVBOSize = meshData->getBufferSize();
		mVBOArray = new uint32_t[mVBOSize];
		glGenBuffers(mVBOSize, mVBOArray);

		int vbo_index = 0;
		if (!meshData->vertices.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->vertexSize(), meshData->vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexPosition::VP_Position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexPosition::VP_Position);
		}

		if (!meshData->normals.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->normalSize(), meshData->normals.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexPosition::VP_Normal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexPosition::VP_Normal);
		}

		if (!meshData->colors.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->colorSize(), meshData->colors.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexPosition::VP_Color, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glEnableVertexAttribArray(VertexPosition::VP_Color);
		}

		if (!meshData->uv.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->uvSize(), meshData->uv.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexPosition::VP_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
			glEnableVertexAttribArray(VertexPosition::VP_UV);
		}

		if (!meshData->indices.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBOArray[vbo_index]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->indexSize(), meshData->indices.data(), GL_STATIC_DRAW);
		}
	}
	*/

	void GLVertexBuffer::setData(const void* data, size_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void GLVertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
	}

	void GLVertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//--------------------------------------------------
	//
	//
	//
	//
	GLIndexBuffer::GLIndexBuffer()
	{
		glGenBuffers(1, &mBufferID);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &mBufferID);
	}

	void GLIndexBuffer::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
	}

	void GLIndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GLIndexBuffer::init(const void* data, const size_t& dataLenght)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataLenght, data, GL_STATIC_DRAW);
	}

	//-------------------------------------------------
	//
	//
	//
	//
	GLVertex::GLVertex()
	{
		glGenVertexArrays(1, &mUID);
	}

	GLVertex::~GLVertex()
	{
		glDeleteVertexArrays(1, &mUID);
	}

	void GLVertex::init(const std::string& name
		, const size_t& vertexCount
		, const DrawMode& drawMode)
	{
		Vertex::init(name, vertexCount, drawMode);
	}

	void GLVertex::bind()
	{
		glBindVertexArray(mUID);
	}

	void GLVertex::unbind()
	{
		glBindVertexArray(0);
	}

	void GLVertex::setVertexBuffer(VertexBuffer* buffer)
	{
		Vertex::setVertexBuffer(buffer);
		auto layout_data = buffer->getLayoutData();
		switch (layout_data.type)
		{
		case  VertexLayoutType::Float1:
		case  VertexLayoutType::Float2:
		case  VertexLayoutType::Float3:
		case  VertexLayoutType::Float4:
			glEnableVertexAttribArray(layout_data.position);
			glVertexAttribPointer(layout_data.position
				, VertexLayout::getTypeLength(layout_data.type)
				, GL_FLOAT
				, layout_data.nomarlized ? GL_TRUE : GL_FALSE
				, layout_data.stride
				, (const void*)0);
		default:
			break;
		}
	}

	void GLVertex::setIndexBuffer(IndexBuffer* buffer)
	{
		Vertex::setIndexBuffer(buffer);
	}

	//-------------------------------------------------
	//
	//	GLVertexCreator
	//
	GLVertexCreator::GLVertexCreator()
	{

	}

	GLVertexCreator::~GLVertexCreator()
	{

	}

	VertexBuffer* GLVertexCreator::createVertexBuffer()
	{
		return GLVertexBuffer::create();
	}

	Vertex* GLVertexCreator::createVertex()
	{
		return GLVertex::create();
	}

	IndexBuffer* GLVertexCreator::createIndexBuffer()
	{
		return GLIndexBuffer::create();
	}
}
