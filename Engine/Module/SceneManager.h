#pragma once
#include <stack>
#include <unordered_map>
#include <string>
#include "Tools.h"

namespace tezcat::Tiny::Module
{
	class Scene;
	class SceneManager
	{
		CREATE_DELEGATE_INSTANCE(SceneManager)
	public:
		SceneManager();
		~SceneManager();

		void init();
		void update();

	public:
		void prepareScene(Scene* scene);

		void pushScene(const std::string& name);
		void popScene();

		Scene* getCurrentScene() { return m_Scenes.top(); }

	private:
		std::stack<Scene*> m_Scenes;
		std::unordered_map<std::string, Scene*> m_SceneWithName;
	};
}