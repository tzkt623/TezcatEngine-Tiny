#pragma once
#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Component/Transform.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Scene;
	/// <summary>
	/// GameObject的生命周期
	/// 属于储存他的那个对象
	///
	/// 子object的生命周期,归父object管
	/// rootobject的生命周期归scene管
	/// </summary>
	class TINY_API GameObject : public TinyObject
	{
		friend class Component;
		friend class Transform;

		GameObject();
		GameObject(const std::string& name);

		TINY_Factory(GameObject);
		TINY_RTTI_H(GameObject);
	public:

		virtual ~GameObject();

	public:

		std::string getName() const { return mName; }
		void setName(const std::string& val) { mName = val; }

		Scene* getScene() { return mScene; }
		void enterScene(Scene* scene);
		void exitScene();

		uint32_t getLayerMask() { return 1 << mLayerMaskIndex; }
		uint32_t getLayerIndex() { return mLayerMaskIndex; }
		void setLayerMaskIndex(uint32_t maskIndex) { mLayerMaskIndex = maskIndex; }

	public:
		int getUID() const { return mUID; }

		Transform* getTransform()
		{
			return static_cast<Transform*>(mComponentList[0]);
		}

		template<class Com>
		Com* getComponent()
		{
			for (auto const& com : mComponentList)
			{
				if (com->is<Com>())
				{
					return static_cast<Com*>(com);
				}
			}

			return nullptr;
		}

		template<class Com>
		Com* getComponentInChildren()
		{
			for (auto const& com : mComponentList)
			{
				if (com->is<Com>())
				{
					return static_cast<Com*>(com);
				}
			}

			auto& children = this->getTransform()->getChildren();
			Com* result = nullptr;
			for (auto child : children)
			{
				result = child->getGameObject()->getComponentInChildren<Com>();
				if (result != nullptr)
				{
					return result;
				}
			}

			return nullptr;
		}

		template<typename Com, typename... Args>
		Com* addComponent(Args&&... args)
		{
			auto component = Com::create(std::forward<Args>(args)...);
			this->addComponent(component);
			return component;
		}

		void addComponent(Component* component);

		template<typename Com>
		void removeComponent()
		{
			auto it = mComponentList.begin();
			while (it != mComponentList.end())
			{
				if ((*it)->is<Com>())
				{
					(*it)->onDetach();
					mComponentList.erase(it);
					break;
				}
			}
		}

		void detachAllComponent() { mComponentList.clear(); }

		TinyVector<Component*>& getCompoents() { return mComponentList; }

	private:
		void removeComponent(Component* com)
		{
			auto it = mComponentList.begin();
			while (it != mComponentList.end())
			{
				if (*it == com)
				{
					it = mComponentList.erase(it);
				}
				else
				{
					(*it)->onComponentRemoved(com);
				}
			}
		}

		void swapTransform();

	public:
		template<typename T>
		static T* instantiate()
		{
			GameObject go = new GameObject();
			return go->addComponent<T>();
		}

		template<typename T>
		static T* instantiate(const std::string& name)
		{
			GameObject go = new GameObject(name);
			return go->addComponent<T>();
		}

		template<typename T>
		static T* instantiate(Transform* parent)
		{
			GameObject go = new GameObject();
			go->getTransform()->setParent(parent);
			return go->addComponent<T>();
		}

		template<typename T>
		static T* instantiate(const std::string& name, Transform* parent)
		{
			GameObject go = new GameObject(name);
			go->getTransform()->setParent(parent);
			return go->addComponent<T>();
		}

	private:
		std::string mName;
		int mUID;
		uint32_t mLayerMaskIndex;

		bool mIsLogic;

	private:
		Scene* mScene;
		TinyVector<Component*> mComponentList;
	};


	class GameObjectPool
	{
		GameObjectPool() = delete;
		~GameObjectPool() = delete;
	public:

		static int addGameObject(GameObject* gameObject);
		static void removeGameObject(GameObject* gameObject);
		static GameObject* get(uint32_t index) { return sPool[index]; }

	private:
		static TinyVector<GameObject*> sPool;
		static std::deque<int> sFreeObjects;
	};
}

