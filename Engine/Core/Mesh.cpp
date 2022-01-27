#include "Mesh.h"
#include "RCVAO.h"

namespace tezcat::Tiny::Core
{
	Mesh::Mesh():
		m_VAO(nullptr)
	{

	}

	Mesh::~Mesh()
	{
		delete m_VAO;
	}

	int Mesh::getBufferSize()
	{
		int count = 0;
		if (!this->vertices.empty())
		{
			count++;
		}

		if (!this->normals.empty())
		{
			count++;
		}

		if (!this->colors.empty())
		{
			count++;
		}

		if (!this->uv.empty())
		{
			count++;
		}

		if (!this->indices.empty())
		{
			count++;
		}

		return count;
	}

	void Mesh::apply()
	{
		m_VAO = new RCVAO();
		m_VAO->createMesh(this);
	}
}


