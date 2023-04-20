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
		bool update();

	public:
		void prepareScene(Scene* scene);

		void pushScene(const std::string& name);
		void pushScene(Scene* scene);

		void popScene();

		Scene* getCurrentScene() { return mScenes.top(); }
		std::unordered_map<std::string, Scene*>& getAllScenes() { return mSceneWithName; }

		bool empty() { return mScenes.empty(); }

	private:
		std::stack<Scene*> mScenes;
		std::unordered_map<std::string, Scene*> mSceneWithName;


	public:
	};

	using SceneMgr = SG<SceneManager>;
}
