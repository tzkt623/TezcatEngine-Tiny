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
		const int CMD_Pop = 0;
		const int CMD_Push = 1;

		enum class CMD
		{
			Pop,
			Push,
		};

		struct CMDData
		{
			CMD cmd;
			Scene* scene;
		};
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

		bool mNeedSwith;
		Scene* mPopScene;
		Scene* mPushScene;
		Scene* mPauseScene;

		std::deque<CMDData> mCDMs;
	};

	using SceneMgr = SG<SceneManager>;
}
