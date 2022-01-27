#pragma once

#include <list>
#include <string>
#include <vector>
#include <unordered_map>

namespace tezcat::Tiny::Module
{
	class Material;
	class GameObject;
	class Camera;
}

namespace tezcat::Tiny::Core
{
	class PassQueue;
	class Pass;
	class RenderObject;
	class GameObject;
	class GUI;
	class Engine;
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void init(Engine* engine);

	public:
		void addPass(Pass* pass);
		Pass* getPass(int id);
		Pass* getPass(const std::string& name);

	public:
		void render();
		void dispatch(Module::GameObject* renderObject);

	private:
		std::list<Pass*> m_PassList;
		std::unordered_map<int, Pass*> m_PassWithID;
		std::unordered_map<std::string, Pass*> m_PassWithName;

		GUI* m_GUI;
	};
}