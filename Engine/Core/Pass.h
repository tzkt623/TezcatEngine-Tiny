#pragma once
#include <memory>
#include <list>
#include <string>

namespace tezcat::Tiny::Module
{
	class Camera;
	class Material;
}

namespace tezcat::Tiny::Core
{
	class RenderObject;
	class Shader;
	class Pass
	{
	public:
		Pass(Shader* program);
		Pass(Module::Material* material);
		~Pass();

		void addRenderObject(RenderObject* renderObject);

	public:
		void render(Module::Camera* camera);

		int getID() const;
		const std::string& getName() const;


	public:


	private:
		Shader* m_Shader;
		Module::Material* m_Material;
		std::list<RenderObject*> m_ObjectList;
	};
}

