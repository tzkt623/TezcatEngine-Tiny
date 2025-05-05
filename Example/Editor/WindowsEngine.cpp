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
#include "WindowsEngine.h"
#include "GUI/GUI.h"
#include "MyGUI/Window/EditorMainWindow.h"

#include <semaphore>
namespace tezcat::Editor
{
	//-----------------------------------------------------------------
	//
	//	WindowsEditor
	//
	bool WindowsEditor::preInit()
	{
		TINY_LOG_ENGINE("Editor Pre Init.....");
		return Engine::preInit();
	}

	bool WindowsEditor::onInit()
	{
		TINY_LOG_ENGINE("Editor On Init.....");
		return Engine::onInit();
	}

	bool WindowsEditor::postInit()
	{
		TINY_LOG_ENGINE("Editor Post Init.....");
		mWindow = ((GLGraphics*)mGraphics)->getGLFWwindow();
		mInputSystem->setWindow(mWindow);

		mGUI = new GUI();
		mGUI->init(mWindow);

		EditorContext::init();
		mEditorWindow = new EditorMainWindow();
		mEditorWindow->open(mGUI);
		mEditorWindow->init();

		return Engine::postInit();
	}

	void WindowsEditor::beforeLoop()
	{
		TINY_LOG_ENGINE("Editor Before Loop.....");
		Engine::beforeLoop();
		mTimeOld = glfwGetTime();
		mTimeNow = 0;
		mPipeline->init();
	}

	void WindowsEditor::endLoop()
	{
		TINY_LOG_ENGINE("Editor End Loop.....");
		Engine::endLoop();
		glfwTerminate();
	}

	void WindowsEditor::preUpdate()
	{
		Engine::preUpdate();
		mTimeNow = glfwGetTime();
		EngineContext::DeltaTime = static_cast<float>(mTimeNow - mTimeOld);
		mTimeOld = mTimeNow;
	}

	void WindowsEditor::onUpdate()
	{
		glfwPollEvents();
		//mInputSystem->update();
		if (SceneManager::update())
		{
			SceneManager::getCurrentScene()->update();

			if(EditorContext::IsPlaying && !EditorContext::IsPause)
			{
				if (EditorContext::IsFocusOnGameView)
				{
					InputSys::getInstance()->update();
				}
				SceneManager::getCurrentScene()->logic();
			}
		}
	}

	void WindowsEditor::notifyRender()
	{
		mGUI->render();
		mPipeline->render();
		glfwSwapBuffers(mWindow);
	}

	void WindowsEditor::postUpdate()
	{
		Engine::postUpdate();
		mIsRunning = !glfwWindowShouldClose(mWindow);
	}

	BaseGraphics* WindowsEditor::createGraphics()
	{
		switch (EngineConfig::getRenderAPI())
		{
		case RenderAPI::OpenGL: return new GLGraphics();
		default:
			throw std::logic_error("RenderAPI must not null");
			break;
		}
	}
}
