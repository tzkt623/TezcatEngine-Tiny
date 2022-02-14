#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class GameObject;
	class Camera;
	class Light;
	class LightData;
	class Transform;
	class Layer;
	class TINY_API Scene
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
		void addTransform(Transform* transform);

		void addCamera(Camera* camera);
		void addGameObject(GameObject* gameObject);

	public:
		void addLogicFunction(void* gameObject, const std::function<void()>& function);
		void removeLogicFunction(void* gameObject);
		void foreachChild(const std::function<void()>& begin,
			const std::function<void(GameObject*)>& get,
			const std::function<void()>& end,
			const std::function<void()>& nochildren);

	public:
		inline std::string getName() const { return m_Name; }
		inline std::unordered_map<std::string, Camera*>* getCameras() { return &m_CameraWithName; }
		inline std::vector<Layer*>& getLayers() { return m_LayerList; }
		inline std::list<GameObject*>& getObjectList() { return m_ObjectList; }
	private:
		std::string m_Name;

		std::list<GameObject*> m_ObjectList;
		std::list<GameObject*> m_NewObjectList;
		std::list<Transform*> m_TransformList;
		std::vector<Layer*> m_LayerList;

		std::unordered_map<void*, std::function<void()>> m_LogicList;

	private://光源信息
		LightData* m_LightData;

	private:
		std::unordered_map<std::string, Camera*> m_CameraWithName;
	};
}

