#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class GameObject;
	class Camera;
	class CameraData;
	class LightData;
	class Transform;
	class Layer;
	class RenderLayer;
	class LightLayer;
	class TINY_API Scene
	{
		friend class GameObject;
	public:
		Scene(const std::string& name);
		virtual ~Scene();
		inline const std::string& getName() const { return mName; }

	public:
		virtual void onEnter();
		virtual void onExit();
		virtual void onPause();
		virtual void onResume();
	public:
		void update();
		void addCamera(Camera* camera);

	public:
		void addLogicFunction(void* gameObject, const std::function<void()>& function);
		void removeLogicFunction(void* gameObject);
		void addParentChangedTransform(Transform* transform);
		void addTransform(Transform* transform);

	private:
		void addGameObject(GameObject* gameObject);
		void addNewObject(GameObject* gameObject);


	public:
		inline const std::list<GameObject*> getObjectList() const { return mObjectList; }
	private:
		std::string mName;
		std::list<GameObject*> mObjectList;
		std::list<GameObject*> mNewObjectList;

		std::list<Transform*> mParentChangedTransformList;
		std::list<Transform*> mTransformList;

		std::unordered_map<void*, std::function<void()>> mLogicList;

		std::vector<RenderLayer*> m_RenderLayerList;
		std::vector<LightLayer*> m_LightLayerList;

	private:
		//灯光数据
		LightData* mLightData;
		//相机数据
		CameraData* mCameraData;
	};
}

