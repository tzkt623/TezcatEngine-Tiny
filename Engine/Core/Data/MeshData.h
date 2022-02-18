#pragma once

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class TINY_API MeshData
	{
	public:
		MeshData(const std::string& name);
		~MeshData();

		size_t vertexSize()
		{
			return this->vertices.size() * sizeof(glm::vec3);
		}

		size_t normalSize()
		{
			return this->normals.size() * sizeof(glm::vec3);
		}

		size_t colorSize()
		{
			return this->colors.size() * sizeof(glm::vec4);
		}

		size_t uvSize()
		{
			return this->uv.size() * sizeof(glm::vec2);
		}

		size_t indexSize()
		{
			return this->indices.size() * sizeof(unsigned int);
		}

		int getBufferSize();

		const std::string& getName() { return m_Name; }

		void apply();

	public:
		std::string m_Name;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec4> colors;
		std::vector<glm::vec2> uv;

		std::vector<unsigned int> indices;
	};
}

