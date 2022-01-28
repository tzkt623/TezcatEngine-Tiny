#include "MeshData.h"
#include "VAO.h"

namespace tezcat::Tiny::Core
{
	MeshData::MeshData(const std::string& name) :
		m_Name(name)
	{

	}

	MeshData::~MeshData()
	{

	}

	int MeshData::getBufferSize()
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

	void MeshData::apply()
	{
		(new VAO())->createMesh(this);
	}
}


