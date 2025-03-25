#pragma once

/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
		static bool isSceneRunning() { return !sSceneArray.empty(); }

	public:
		static Scene* getCurrentScene() { return sSceneArray.top(); }

		static bool empty() { return sSceneArray.empty(); }
		static void popScene();
		static void pushScene(const std::string& name);
		static void pushScene(Scene* scene);
		static void prepareScene(Scene* scene);
		static void switchScene(Scene* scene);
		static const std::unordered_map<std::string_view, Scene*>& getAllSceneMap() { return sPreparedSceneUMap; }
		static const std::vector<Scene*>& getAllSceneArray() { return sPreparedSceneArray; }

	private:
		static std::stack<Scene*> sSceneArray;
		static std::deque<CMDData> mCDMs;

		static std::unordered_map<std::string_view, Scene*> sPreparedSceneUMap;
		static std::vector<Scene*> sPreparedSceneArray;
	};
}
