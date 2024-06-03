#pragma once

#include "../Head/TinyCpp.h"
#include "../Base/TinyObject.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class GameObject;
	class GameObjectData;
	class Camera;
	class CameraData;
	class LightData;
	class Transform;
	class Layer;
	class RenderObjectCache;
	class LightLayer;
	class BaseGraphics;
	class TINY_API Scene : public TinyObject
	{
		friend class GameObject;
		friend class Transform;
		TINY_ABSTRACT_OBJECT_H(Scene, TinyObject)

	protected:
		Scene(std::string name);

	public:
		virtual ~Scene();
		std::string_view getName() { return mName; }

		void prepare();
		void open();
		void push();

	public:
		virtual void onEnter();
		virtual void onExit();
		virtual void onPause();
		virtual void onResume();

	public:
		void update();

	public:
		void addLogicFunction(void* gameObject, const std::function<void()>& function);
		void removeLogicFunction(void* gameObject);
		void addTransform(Transform* transform);

	public:
		std::list<TinyWeakRef<Transform>>& getTransformList() { return mTransformList; }
		std::vector<TinyWeakRef<Transform>> findTransforms(const std::string& name);
		TinyWeakRef<Transform> findTransform(const std::string& name);

	private:
		int32 addGameObject(GameObject* gameObject);
		void removeGameObject(GameObject* gameObject);

	private:
		std::string mName;
		std::list<GameObject*> mNewGameObjectList;
		std::unordered_map<void*, std::function<void()>> mLogicList;

	public:
		std::list<TinyWeakRef<Transform>> mTransformList;
	private:
		//灯光数据
		LightData* mLightData;
		//相机数据
		CameraData* mCameraData;
		//
		GameObjectData* mGameObjectData;
	};
}

