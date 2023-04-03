#pragma once
#include "../Head/CppHead.h"
#include "Utility/Utility.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Scene;
	class TINY_API SceneManager
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

		Scene* getCurrentScene() { return mScenes.top(); }

	private:
		std::stack<Scene*> mScenes;
		std::unordered_map<std::string, Scene*> m_SceneWithName;
	};

	using SceneMgr = SG<SceneManager>;
}