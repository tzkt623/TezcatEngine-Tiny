#include "GLVertexBuffer.h"
#include "GLHead.h"
#include "Core/Data/MeshData.h"
#include "Core/Manager/VertexGroupManager.h"

namespace tezcat::Tiny::GL
{
	GLVertexBuffer::GLVertexBuffer(MeshData* meshData)
		: mVBOSize(0)
		, mVBOArray(nullptr)
	{
		this->init(meshData);
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

			glVertexAttribPointer(VertexMask::Position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexMask::Position);
		}

		if (!meshData->normals.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->normalSize(), meshData->normals.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexMask::Normal);
		}

		if (!meshData->colors.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->colorSize(), meshData->colors.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::Color, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glEnableVertexAttribArray(VertexMask::Color);
		}

		if (!meshData->uv.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->uvSize(), meshData->uv.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::UV, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
			glEnableVertexAttribArray(VertexMask::UV);
		}

		if (!meshData->indices.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBOArray[vbo_index]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->indexSize(), meshData->indices.data(), GL_STATIC_DRAW);
		}
	}

	//
	GLVertexGroup::GLVertexGroup(MeshData* meshData)
	{
		this->init(meshData);
	}

	GLVertexGroup::~GLVertexGroup()
	{
		delete mVertexBuffer;
	}

	void GLVertexGroup::init(MeshData* meshData)
	{
		if (mUID != 0)
		{
			return;
		}

		glGenVertexArrays(1, &mUID);
		glBindVertexArray(mUID);

		mVertexBuffer = new GLVertexBuffer(meshData);

		glBindVertexArray(0);

		mName = meshData->getName();
		mVertexCount = static_cast<int>(meshData->vertices.size());
		mIndexCount = static_cast<int>(meshData->indices.size());

		VertexGroupMgr::getInstance()->addVertexGroup(this);
	}

	void GLVertexGroup::bind()
	{
		glBindVertexArray(mUID);
	}

	void GLVertexGroup::unbind()
	{
		glBindVertexArray(0);
	}

	VertexBuffer* GLVertexBufferCreator::create(MeshData* meshData)
	{
		return new GLVertexBuffer(meshData);
	}

	VertexGroup* GLVertexGroupCreator::create(MeshData* meshData)
	{
		return new GLVertexGroup(meshData);
	}

}
