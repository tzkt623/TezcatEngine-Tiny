#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Component/Transform.h"
#include "../Base/BaseObject.h"

namespace tezcat::Tiny::Core
{
	class Scene;
	class TINY_API GameObject : BaseObject
	{
		friend class Component;
		friend class Transform;
	public:
		GameObject();
		GameObject(const std::string& name);
		virtual ~GameObject();

	public:
		virtual void close() override;

		std::string getName() const { return mName; }
		void setName(const std::string& val) { mName = val; }

		Scene* getScene() { return mScene; }
		void enterScene(Scene* scene);
		void exitScene();

		uint32_t getLayerMask() { return 1 << mLayerMaskIndex; }
		uint32_t getLayerMaskIndex() { return mLayerMaskIndex; }
		void setLayerMaskIndex(uint32_t maskIndex) { mLayerMaskIndex = maskIndex; }

	public:
		bool needDelete() const { return mNeedDelete; }

	public:
		int getUID() const { return mUID; }

		Transform* getTransform()
		{
			return mTransform;
//			return static_cast<Transform*>(mComponentList[0]);
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
			auto component = new Com(std::forward<Args>(args)...);
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

		std::vector<Component*>& getCompoents() { return mComponentList; }

	private:
		void removeComponent(Component* com)
		{
			auto it = mComponentList.begin();
			while (it != mComponentList.end())
			{
				if ((*it) == com)
				{
					it = mComponentList.erase(it);
				}

				(*it)->onComponentRemoved(com);
			}
		}

		void swapTransform(Transform* transform);

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

		static void destory(GameObject* go);
		static void clearDeletedGameObjects();



	private:
		std::string mName;
		int mUID;
		uint32_t mLayerMaskIndex;

		bool mIsLogic;
		bool mNeedDelete;

	private:
		Scene* mScene;
		Transform* mTransform;
		std::vector<Component*> mComponentList;

	private:
		static std::stack<int> sUIDPool;
		static std::list<GameObject*> sDeleteObjectList;
	};
}

