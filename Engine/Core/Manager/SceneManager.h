#pragma once
#include "../Head/CppHead.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	class Scene;
	class SceneManager
	{
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

	using SceneMgr = SG<SceneManager>;
}