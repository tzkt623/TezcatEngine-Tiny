#pragma once

#include "CommonHead.h"

namespace tezcat::Tiny::Module
{
	class Material;
	class GameObject;
	class Camera;
}

namespace tezcat::Tiny::Core
{
	class Pass_Shader;
	class RenderAgent;
	class GUI;
	class Engine;
	class VAOManager;

	//------------------------------------------
	//
	//	Rednerer
	//	|
	//	Pass
	//	|
	//	----light->camera
	//	|
	//	VAOPass
	//	|
	//	RenderCommand
	//
	//
	//
	class Renderer
	{
		CREATE_DELEGATE_INSTANCE(Renderer)
	public:
		Renderer();
		~Renderer();

		void init(Engine* engine);

	public:
		void addPass(Pass_Shader* pass);
		Pass_Shader* getPass(int id);
		Pass_Shader* getPass(const std::string& name);

	public:
		void render();
		void dispatch(RenderAgent* renderAgent);

	private:
		std::list<Pass_Shader*> m_PassList;
		std::unordered_map<int, Pass_Shader*> m_PassWithID;
		std::unordered_map<std::string, Pass_Shader*> m_PassWithName;

		GUI* m_GUI;
		VAOManager* m_VAOManager;
	};
}