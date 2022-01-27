#pragma once
#include <memory>
#include <list>
#include <string>

namespace tezcat::Tiny::Module
{
	class Camera;
}

namespace tezcat::Tiny::Core
{
	class RenderObject;
	class Shader;
	class Pass
	{
	public:
		Pass(Shader* program);
		~Pass();

		void addRenderObject(RenderObject* renderObject);

	public:
		void render(Module::Camera* camera);

		int getID() const;
		const std::string& getName() const;


	public:


	private:
		Shader* m_Shader;
		std::list<RenderObject*> m_ObjectList;
	};
}

