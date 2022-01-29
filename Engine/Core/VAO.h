#pragma once
#include "CommonHead.h"
#include "RenderCommand.h"

namespace tezcat::Tiny::Core
{
	class MeshData;
	class VAO : public RenderCommand
	{
	public:
		VAO();
		~VAO();

		std::string getName() const { return m_Name; }
		void setName(const std::string& val) { m_Name = val; }

		int getVertexCount() const { return m_VertexCount; }
		int getIndexCount() const { return m_IndexCount; }

	public:
		unsigned int getID() const { return m_ID; }
		void bind();

	public:
		void createMesh(MeshData *mesh);

	private:
		std::string m_Name;

		int m_VertexCount;
		int m_IndexCount;

		unsigned int m_ID;
		unsigned int* m_VBOArray;
		int m_VBOSize;
	};
}