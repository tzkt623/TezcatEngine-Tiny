#pragma once
#include "../Head/TinyCpp.h"
#include "../Tool/Tool.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class Scene;
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
		SceneManager() = delete;
		~SceneManager() = delete;

	public:
		static void init();
		static bool update();

	public:
		static Scene* getCurrentScene() { return sSceneArray.top(); }

		static bool empty() { return sSceneArray.empty(); }
		static void popScene();
		static void pushScene(const std::string& name);
		static void pushScene(Scene* scene);
		static void prepareScene(Scene* scene);
		static void switchScene(Scene* scene);
		static const std::unordered_map<std::string_view, Scene*>& getAllScene() { return sSceneUMap; }

	private:
		static std::stack<Scene*> sSceneArray;
		static std::unordered_map<std::string_view, Scene*> sSceneUMap;
		static std::deque<CMDData> mCDMs;
	};
}
