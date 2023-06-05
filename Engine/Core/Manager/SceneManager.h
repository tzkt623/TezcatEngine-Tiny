#pragma once
#include "../Head/TinyCpp.h"
#include "../Tool/Tool.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class Scene;
	class BaseGraphics;
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

		void switchScene(Scene* scene);

		void popScene();

		Scene* getCurrentScene() { return mScenes.top(); }
		TinyUMap<std::string, Scene*>& getAllScenes() { return mSceneWithName; }

		bool empty() { return mScenes.empty(); }

	private:

		TinyStack<Scene*> mScenes;
		TinyUMap<std::string, Scene*> mSceneWithName;
	};

	using SceneMgr = SG<SceneManager>;
}
