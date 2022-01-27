#include "RCVAO.h"
#include "Mesh.h"

#include <glad/glad.h>



namespace tezcat::Tiny::Core
{
	RCVAO::RCVAO() :
		m_ID(0),
		m_VBOArray(nullptr),
		m_VBOSize(0)
	{

	}

	RCVAO::~RCVAO()
	{
		glDeleteBuffers(m_VBOSize, m_VBOArray);
		glDeleteVertexArrays(1, &m_ID);
		delete[] m_VBOArray;
		m_VBOArray = nullptr;
	}

	void RCVAO::bind()
	{
		glBindVertexArray(m_ID);
	}

	void RCVAO::createMesh(Mesh* mesh)
	{
		if (m_ID != 0)
		{
			return;
		}

		glGenVertexArrays(1, &m_ID);
		glBindVertexArray(m_ID);

		m_VBOSize = mesh->getBufferSize();
		m_VBOArray = new unsigned int[m_VBOSize];
		glGenBuffers(m_VBOSize, m_VBOArray);

		int vbo_index = 0;
		if (!mesh->vertices.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, mesh->vertexSize(), mesh->vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::Position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexMask::Position);
		}

		if (!mesh->normals.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, mesh->normalSize(), mesh->normals.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(VertexMask::Normal);
		}

		if (!mesh->colors.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, mesh->colorSize(), mesh->colors.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::Color, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glEnableVertexAttribArray(VertexMask::Color);
		}

		if (!mesh->uv.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBOArray[vbo_index++]);
			glBufferData(GL_ARRAY_BUFFER, mesh->uvSize(), mesh->uv.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(VertexMask::UV, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
			glEnableVertexAttribArray(VertexMask::UV);
		}

		if (!mesh->indices.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOArray[vbo_index]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexSize(), mesh->indices.data(), GL_STATIC_DRAW);
		}


		/*
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->vertexSize(), mesh->vertices.data(), GL_STATIC_DRAW);

		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexSize(), mesh->indices.data(), GL_STATIC_DRAW);

		// 顶点
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		// 法线
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// 纹理坐标
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		*/

		glBindVertexArray(0);
	}
}

