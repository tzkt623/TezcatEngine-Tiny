#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <initializer_list>
#include <xutility>
#include <memory>

namespace tezcat::Tiny::Core
{
	class RCVAO;
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

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

		void apply();

		RCVAO* getRenderCommand() const { return m_VAO; }

	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec4> colors;
		std::vector<glm::vec2> uv;

		std::vector<unsigned int> indices;

	private:
		RCVAO* m_VAO;
	};
}

