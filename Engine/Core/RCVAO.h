#pragma once
#include "RenderCommand.h"
#include <memory>
#include "Config.h"
#include <initializer_list>

namespace tezcat::Tiny::Core
{
	class Mesh;
	class RCVAO : public RenderCommand
	{
	public:
		RCVAO();
		virtual ~RCVAO();

	public:
		unsigned int getID() const { return m_ID; }
		void bind();

	public:
		void createMesh(Mesh *mesh);

	private:
		unsigned int m_ID;
		unsigned int* m_VBOArray;
		int m_VBOSize;
	};
}