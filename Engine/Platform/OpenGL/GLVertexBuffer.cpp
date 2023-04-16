#include "GLVertexBuffer.h"
#include "GLHead.h"
#include "Core/Data/MeshData.h"

namespace tezcat::Tiny::GL
{
	GLVertexBuffer::GLVertexBuffer()
		: mVBOSize(0)
		, mVBOArray(nullptr)
	{

	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(mVBOSize, mVBOArray);
		delete[] mVBOArray;
	}

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

			glVertexAttribPointer(VertexPosition::Position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexPosition::Position);
		}

		if (!meshData->normals.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->normalSize(), meshData->normals.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexPosition::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexPosition::Normal);
		}

		if (!meshData->colors.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->colorSize(), meshData->colors.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexPosition::Color, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glEnableVertexAttribArray(VertexPosition::Color);
		}

		if (!meshData->uv.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->uvSize(), meshData->uv.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexPosition::UV, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
			glEnableVertexAttribArray(VertexPosition::UV);
		}

		if (!meshData->indices.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBOArray[vbo_index]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->indexSize(), meshData->indices.data(), GL_STATIC_DRAW);
		}
	}

	//
	GLVertex::GLVertex()
	{
		glGenVertexArrays(1, &mUID);
	}

	GLVertex::~GLVertex()
	{
		glDeleteVertexArrays(1, &mUID);
		delete mVertexBuffer;
	}

	void GLVertex::init(MeshData* meshData)
	{
		glBindVertexArray(mUID);

		mVertexBuffer = new GLVertexBuffer();
		mVertexBuffer->init(meshData);

		glBindVertexArray(0);

		mName = meshData->getName();
		mVertexCount = static_cast<int>(meshData->vertices.size());
		mIndexCount = static_cast<int>(meshData->indices.size());
	}

	void GLVertex::bind()
	{
		glBindVertexArray(mUID);
	}

	void GLVertex::unbind()
	{
		glBindVertexArray(0);
	}

	//-------------------------------------------------
	//
	//
	//
	//
	GLVertexCreator::GLVertexCreator()
	{

	}

	GLVertexCreator::~GLVertexCreator()
	{

	}

	VertexBuffer* GLVertexCreator::createVertexBuffer()
	{
		return new GLVertexBuffer();
	}

	Vertex* GLVertexCreator::createVertex()
	{
		return new GLVertex();
	}



}
