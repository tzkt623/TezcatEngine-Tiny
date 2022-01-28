#pragma once
#include "RenderObject.h"
#include "GLHead.h"

namespace tezcat::Tiny::Core
{
	class VAO;
	class MeshData;
	class Shader;

	class Mesh : public RenderObject
	{
	public:

	public:
		Mesh();
		Mesh(MeshData* meshData);
		Mesh(VAO* vao);
		virtual ~Mesh();

		void render(Shader* shader) override;
		void apply() override;

		VAO* getVAO() const { return m_VAO; }

	private:
		DrawMode m_DrawMode;
		VAO* m_VAO;
	};
}