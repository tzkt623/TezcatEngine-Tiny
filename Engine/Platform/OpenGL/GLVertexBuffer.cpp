#include "GLVertexBuffer.h"
#include "GLHead.h"
#include "Core/Data/MeshData.h"
#include "Core/Manager/VertexGroupManager.h"

namespace tezcat::Tiny::Core
{
	GLVertexBuffer::GLVertexBuffer(MeshData* meshData)
		: m_VBOSize(0)
		, m_VBOArray(nullptr)
	{
		this->init(meshData);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(m_VBOSize, m_VBOArray);
		delete[] m_VBOArray;
	}

	void GLVertexBuffer::init(MeshData* meshData)
	{
		m_VBOSize = meshData->getBufferSize();
		m_VBOArray = new uint32_t[m_VBOSize];
		glGenBuffers(m_VBOSize, m_VBOArray);

		int vbo_index = 0;
		if (!meshData->vertices.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->vertexSize(), meshData->vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::Position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexMask::Position);
		}

		if (!meshData->normals.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->normalSize(), meshData->normals.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexMask::Normal);
		}

		if (!meshData->colors.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->colorSize(), meshData->colors.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::Color, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glEnableVertexAttribArray(VertexMask::Color);
		}

		if (!meshData->uv.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, meshData->uvSize(), meshData->uv.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::UV, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
			glEnableVertexAttribArray(VertexMask::UV);
		}

		if (!meshData->indices.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOArray[vbo_index]);
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
		delete m_VertexBuffer;
	}

	void GLVertexGroup::init(MeshData* meshData)
	{
		if (m_UID != 0)
		{
			return;
		}

		glGenVertexArrays(1, &m_UID);
		glBindVertexArray(m_UID);

		m_VertexBuffer = new GLVertexBuffer(meshData);

		glBindVertexArray(0);

		m_Name = meshData->getName();
		m_VertexCount = static_cast<int>(meshData->vertices.size());
		m_IndexCount = static_cast<int>(meshData->indices.size());

		VertexGroupMgr::getInstance()->addVertexGroup(this);
	}

	void GLVertexGroup::bind()
	{
		glBindVertexArray(m_UID);
	}

}