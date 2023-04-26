#pragma once

#include "../Head/TinyCpp.h"
#include "../Base/TinyObject.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class GameObject;
	class Camera;
	class CameraData;
	class LightData;
	class Transform;
	class Layer;
	class RenderLayer;
	class LightLayer;
	class TINY_API Scene : public TinyObject
	{
		friend class GameObject;
		friend class Transform;

		TINY_RTTI_H(Scene)

	protected:
		Scene(const std::string& name);

	public:
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
		TinyList<GameObject*>& getObjectList() { return mObjectList; }
		std::list<TinyWeakRef<Transform>>& getTransformList() { return mTransformList; }
		TinyVector<GameObject*> findGameObjects(const std::string& name);

	private:
		uint32_t addUpdateTransform(Transform* transform);
		void setUpdateTransform(const uint32_t& index, Transform* transform);

	private:
		std::string mName;
		std::list<GameObject*> mNewObjectList;
		std::unordered_map<void*, std::function<void()>> mLogicList;

	public:

		TinyList<GameObject*> mObjectList;
		std::list<TinyWeakRef<Transform>> mTransformList;
	private:
		//灯光数据
		LightData* mLightData;
		//相机数据
		CameraData* mCameraData;
	};
}

