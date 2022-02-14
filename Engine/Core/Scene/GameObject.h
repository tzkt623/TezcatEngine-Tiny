#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Component/Transform.h"

namespace tezcat::Tiny::Core
{
	class MeshData;
	class Material;
	class Scene;
	class TINY_API GameObject
	{
	public:
		GameObject();
		GameObject(const std::string& name);
	private:
		~GameObject();

	public:
		std::string getName() const { return m_Name; }
		void setName(const std::string& val) { m_Name = val; }

		void enterScene(Scene* scene);
		void exitScene();

		uint32_t getLayerMask() { return 1 << m_LayerMaskIndex; }
		inline uint32_t getLayerMaskIndex() { return m_LayerMaskIndex; }
		void setLayerMask(uint32_t maskIndex) { m_LayerMaskIndex = 1 << maskIndex; }
		inline Scene* getScene() { return m_Scene; }

	public:
		inline bool needDelete() const { return m_NeedDelete; }

	public:
		inline int getUID() const { return m_UID; }

		inline Transform* getTransform()
		{
			return (Transform*)m_ComponentList[0].get();
		}

		template<class Com>
		Com* getComponent()
		{
			for (auto const& com : m_ComponentList)
			{
				if (com->is<Com>())
				{
					return (Com*)com;
				}
			}

			return nullptr;
		}

		template<class Com>
		Com* getComponentInChildren()
		{
			for (auto const& com : m_ComponentList)
			{
				if (com->is<Com>())
				{
					return (Com*)com;
				}
			}

			auto& children = this->getTransform()->getChildren();
			Com* result = nullptr;
			for (auto child : children)
			{
				result = child->getComponentInChildren<Com>();
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
			auto it = m_ComponentList.begin();
			while (it != m_ComponentList.end())
			{
				if ((*it)->is<Com>())
				{
					(*it)->onDetach();
					m_ComponentList.erase(it);
					break;
				}
			}
		}

		void detachAllComponent() { m_ComponentList.clear(); }

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
		std::string m_Name;
		int m_UID;
		bool m_IsLogic;
		bool m_NeedDelete;
		unsigned int m_LayerMaskIndex;

	private:
		Scene* m_Scene;
		//0号组件必须是Transform
		std::vector<ref<Component>> m_ComponentList;

	private:
		static std::stack<int> s_UIDPool;
		static std::list<GameObject*> s_DeleteObjectList;
	};
}

