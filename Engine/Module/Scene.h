#pragma once

#include "CommonHead.h"

namespace tezcat::Tiny::Module
{
	class GameObject;
	class Camera;
	class Light;
	class Scene
	{
	public:
		Scene(const std::string& name);
		virtual ~Scene();

	public:
		virtual void onEnter();
		virtual void onExit();
		virtual void onPause();
		virtual void onResume();
	public:
		void update();
		void addGameObject(GameObject* go);
		void addCamera(Camera* camera);

	public:
		void addLogicFunction(GameObject* gameObject, const std::function<void()>& function);
		void removeLogicFunction(GameObject* gameObject);
		std::list<GameObject*>& getObjectList() { return m_ObjectList; }

		void foreachChild(const std::function<void()>& begin,
			const std::function<void(GameObject*)>& get,
			const std::function<void()>& end,
			const std::function<void()>& nochildren);

	public:
		std::string getName() const { return m_Name; }
		std::unordered_map<std::string, Camera*>* getCameras() { return &m_CameraWithName; }

	private:
		std::string m_Name;

		std::list<GameObject*> m_NewEnterObjectList;
		std::list<GameObject*> m_ObjectList;
		std::unordered_map<GameObject*, std::function<void()>> m_LogicList;

	private://光源信息

		std::unordered_set<Light*> m_LightList;

	private:
		std::unordered_map<std::string, Camera*> m_CameraWithName;
	};
}

