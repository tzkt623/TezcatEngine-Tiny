#include "SceneManager.h"
#include "../Scene/Scene.h"
#include "../Tool/Tool.h"
#include "../Profiler.h"
#include "../Event/EngineEvent.h"

namespace tezcat::Tiny
{
	SceneManager::SceneManager()
	{
		SG<SceneManager>::attach(this);

		EngineEvent::get()->addListener(EngineEventID::EE_PopScene
			, this
			, [this](const EventData& data)
			{
				this->popScene();
			});

		EngineEvent::get()->addListener(EngineEventID::EE_PushScene
			, this
			, [this](const EventData& data)
			{
				this->pushScene((Scene*)data.userData);
			});
	}

	SceneManager::~SceneManager()
	{
		EngineEvent::get()->removeListener(this);
	}

	void SceneManager::init()
	{

	}

	void SceneManager::pushScene(const std::string& name)
	{
		if (!mScenes.empty())
		{
			mScenes.top()->onPause();
		}

		auto it = mSceneWithName.find(name);
		if (it != mSceneWithName.end())
		{
			mScenes.push(it->second);
			EngineEvent::get()->dispatch({ EngineEventID::EE_OnPushScene });
			it->second->onEnter();
		}
	}

	void SceneManager::pushScene(Scene* scene)
	{
		if (!mScenes.empty())
		{
			if (mScenes.top() == scene)
			{
				return;
			}

			mScenes.top()->onPause();
		}

		mScenes.push(scene);
		EngineEvent::get()->dispatch({ EngineEventID::EE_OnPushScene });
		scene->onEnter();
	}

	void SceneManager::popScene()
	{
		if (mScenes.empty())
		{
			return;
		}

		mScenes.top()->onExit();
		mScenes.pop();
		EngineEvent::get()->dispatch({ EngineEventID::EE_OnPopScene });

		if (!mScenes.empty())
		{
			mScenes.top()->onResume();
		}
	}

	void SceneManager::prepareScene(Scene* scene)
	{
		mSceneWithName.try_emplace(scene->getName(), scene);
	}

	bool SceneManager::update(BaseGraphics* graphics)
	{
		TINY_PROFILER_TIMER_OUT(Profiler::LogicTime);
		if (mScenes.empty())
		{
			return false;
		}

		mScenes.top()->update(graphics);
		return true;
	}

	void SceneManager::switchScene(Scene* scene)
	{
		if (!mScenes.empty())
		{
			mScenes.top()->onExit();
			mScenes.pop();
			EngineEvent::get()->dispatch(EventData{ EngineEventID::EE_OnPopScene });
		}

		mScenes.push(scene);
		EngineEvent::get()->dispatch(EventData{ EngineEventID::EE_OnPushScene });
		scene->onEnter();
	}

}
