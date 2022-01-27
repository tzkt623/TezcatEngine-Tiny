#include "RenderObject.h"

#include "Mesh.h"
#include "glad/glad.h"
#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"


namespace tezcat::Tiny::Core
{
	RenderObject::RenderObject():
		m_Enable(true),
		m_MasterObject(nullptr)
	{

	}

	RenderObject::~RenderObject()
	{

	}

	/// <summary>
	/// 
	/// </summary>

	ROMesh::ROMesh():
		m_VAO(nullptr),
		m_Mesh(nullptr)
	{

	}

	ROMesh::~ROMesh()
	{
		m_Mesh = nullptr;
		m_VAO = nullptr;
	}

	void ROMesh::draw(Shader* shader)
	{
		m_VAO->bind();

		auto transform = this->getMasterObject()->getTransform();
		shader->setModelMatrix(transform->getModelMatrix());

		glDrawElements(GL_TRIANGLES, m_Mesh->indices.size(), GL_UNSIGNED_INT, 0);
		//		glBindVertexArray(0);
	}

	void ROMesh::apply()
	{
		m_VAO = m_Mesh->getRenderCommand();
	}
}


