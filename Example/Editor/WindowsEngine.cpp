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
		return Engine::preInit();
	}

	bool WindowsEditor::onInit()
	{
		return Engine::onInit();
	}

	bool WindowsEditor::postInit()
	{
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
		Engine::beforeLoop();
		mTimeOld = glfwGetTime();
		mTimeNow = 0;
		mPipeline->init();
	}

	void WindowsEditor::endLoop()
	{
		Engine::endLoop();
		glfwTerminate();
	}

	void WindowsEditor::preUpdate()
	{
		Engine::preUpdate();
		mTimeNow = glfwGetTime();
		sDeltaTime = static_cast<float>(mTimeNow - mTimeOld);
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
