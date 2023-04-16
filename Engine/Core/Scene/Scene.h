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
		friend class Transform;
	public:
		Scene(const std::string& name);
		virtual ~Scene();
		const std::string& getName() const { return mName; }

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
		void addTransform(Transform* transform);

	private:
		void addGameObject(GameObject* gameObject);
		void addNewObject(GameObject* gameObject);


	public:
		inline const std::list<GameObject*>& getObjectList() const { return mObjectList; }
		inline const std::list<Transform*>& getTransformList() const { return mTransformList; }
		std::vector<GameObject*> findGameObjects(const std::string& name);

	private:
		uint32_t addUpdateTransform(Transform* transform);
		void setUpdateTransform(const uint32_t& index, Transform* transform);

	private:
		std::string mName;
		std::list<GameObject*> mObjectList;
		std::list<GameObject*> mNewObjectList;
		std::list<Transform*> mTransformList;
		std::vector<Transform*> mUpdateTransformList;

		std::unordered_map<void*, std::function<void()>> mLogicList;
	private:
		//灯光数据
		LightData* mLightData;
		//相机数据
		CameraData* mCameraData;
	};
}

