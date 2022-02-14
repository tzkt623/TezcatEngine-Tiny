#pragma once

#include "Core/Head/CppHead.h"
#include "RenderCommand.h"

namespace tezcat::Tiny::Core
{
	class MeshData;
	class VertexBuffer;
	class VertexGroup
	{
	public:
		VertexGroup();
		virtual ~VertexGroup();

	public:
		std::string getName() const { return m_Name; }
		void setName(const std::string& val) { m_Name = val; }
		int getVertexCount() const { return m_VertexCount; }
		int getIndexCount() const { return m_IndexCount; }
		unsigned int getUID() const { return m_UID; }

	public:
		virtual void initWithMesh(MeshData* mesh) = 0;
		virtual void bind() = 0;

	protected:
		std::string m_Name;
		unsigned int m_UID;

		int m_VertexCount;
		int m_IndexCount;

		VertexBuffer* m_VertexBuffer;
	};
}