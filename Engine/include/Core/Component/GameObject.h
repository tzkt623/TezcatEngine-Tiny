#pragma once
#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

#include "../Component/Transform.h"

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
		friend class Scene;

		GameObject();
		GameObject(std::string name);

		TINY_OBJECT_H(GameObject, TinyObject)
	public:

		virtual ~GameObject();

	protected:
		virtual void init() override;

	public:

		std::string getName() const { return mName; }
		void setName(const std::string& val) { mName = val; }

		Scene* getScene() { return mScene; }
		void enterScene(Scene* scene);
		void exitScene();

		uint32_t getLayerMask() const { return 1 << mLayerMaskIndex; }
		uint32_t getLayerIndex() const { return mLayerMaskIndex; }
		void setLayerMaskIndex(uint32_t maskIndex) { mLayerMaskIndex = maskIndex; }
		std::string getMemoryInfo() override;

	public:
		int32_t getUID() const { return mUID; }

		Transform* getTransform()
		{
			return static_cast<Transform*>(mComponentList[Transform::getComponentTypeIDStatic()]);
		}

		template<class Com>
		Com* getComponent()
		{
			for (auto const& com : mComponentList)
			{
				if (com->typeIDEqual<Com>())
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
				if (com->typeIDEqual<Com>())
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

		std::vector<Component*>& getCompoents() { return mComponentList; }

	private:
		void removeComponent(Component* com);
		void swapTransform();

	protected:
		virtual void onClose() override;

	private:
		bool mIsLogic;
		std::string mName;
		int32_t mUID;
		uint32_t mLayerMaskIndex;

	private:
		Scene* mScene;
		std::vector<Component*> mComponentList;
	};
}

