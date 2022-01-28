#include "Mesh.h"
#include "MeshData.h"
#include "Material.h"
#include "Shader.h"
#include "VAO.h"
#include "GameObject.h"
#include "Transform.h"

namespace tezcat::Tiny::Core
{
	Mesh::Mesh() :
		Mesh((VAO*)nullptr)
	{

	}

	Mesh::Mesh(MeshData* meshData) :
		Mesh(new VAO())
	{
		m_VAO->createMesh(meshData);
	}

	Mesh::Mesh(VAO* vao) :
		m_VAO(vao),
		m_DrawMode(DrawMode::Triangles)
	{

	}

	Mesh::~Mesh()
	{
		m_VAO = nullptr;
	}

	void Mesh::render(Shader* shader)
	{
		if (m_VAO->getIndexCount() > 0)
		{
			glDrawElements(m_DrawMode, m_VAO->getIndexCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(m_DrawMode, 0, m_VAO->getVertexCount());
		}
	}

	void Mesh::apply()
	{

	}
}