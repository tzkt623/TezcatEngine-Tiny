/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "EditorEngineIniter.h"
#include "EditorInputer.h"
#include "EditorEvent.h"

#include "Scene/SceneHead.h"


using namespace tezcat::Tiny::GL;
#define CreateWindow(host, X) (new X())->open(host)
namespace tezcat::Editor
{
	EditorEngineIniter::EditorEngineIniter()
		: EngineIniter(RenderAPI::OpenGL)
	{

	}

	void EditorEngineIniter::prepareEngine(Engine* engine)
	{
		TINY_LOG_ENGINE("Prepare Engine.....");
		EngineIniter::prepareEngine(engine);
		EditorEvent::get()->init(EditorEventID::Count);

		//engine->setEnableMultiThread();

		//mResourceFolderName = "Resource";
		//mGameName = u8"V0.3(没有黑夜君临玩我要死了)";
		EngineContext::Name = u8"V0.3(没有黑夜君临玩我要死了)";
		EngineContext::ScreenWidth = 1920;
		EngineContext::ScreenHeight = 1080;
		EngineContext::EnableVsync = true;

		this->setGLVersion(4, 3);
	}

	void EditorEngineIniter::prepareResource(Engine* engine)
	{
		TINY_LOG_ENGINE("Prepare Resource.....");
		EngineIniter::prepareResource(engine);
		ShaderManager::loadShaderFiles("Resource/Shaders/Tutorial");
	}

	void EditorEngineIniter::prepareGame(Engine* engine)
	{
		TINY_LOG_ENGINE("Prepare Scene.....");
		EngineIniter::prepareGame(engine);

		MyMainScene::create("MainScene")->prepare();
		SceneHelper::prepareTutorialScene();
	}

	void EditorEngineIniter::initYourShaderParam()
	{
		//std::cout << "Init My Shader Params\n";
	}
}
