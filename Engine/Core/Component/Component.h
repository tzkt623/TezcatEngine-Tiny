#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"


namespace tezcat::Tiny
{
#define LogicBind(func) std::bind(&func, this)

	class GameObject;
	class Transform;

	class TINY_API Component : public TinyObject
	{
		friend class GameObject;

	public:
		Component();
		virtual ~Component();

		/// <summary>
		/// 启动逻辑
		/// </summary>
		void startLogic(const std::function<void()>& logicFunction);

		/// <summary>
		/// 停止逻辑
		/// </summary>
		void stopLogic();

	protected:
		//进入场景时执行
		virtual void onStart()
		{

		}

		//加入GameObject/手动启用时执行
		virtual void onEnable()
		{

		}

		//退出场景/手动操作时执行
		virtual void onDisable()
		{

		}

		//析构函数执行
		virtual void onDestroy();

		//virtual void onUpdate() {}

		void onDetach()
		{
			mEnable = false;
			this->onDisable();
			mGameObject = nullptr;
		}

	public:
		GameObject* getGameObject() const { return mGameObject; }
		void setGameObject(GameObject* val) { mGameObject = val; }

		Transform* getTransform();

		bool isEnable() const { return mEnable; }
		void setEnable(bool val)
		{
			mEnable = val;
			if (mEnable)
			{
				this->onEnable();
			}
			else
			{
				this->onDisable();
			}
		}

		bool isDetached() const { return mGameObject == nullptr; }

	public:
		virtual void onComponentAdded(Component* component) {}
		virtual void onComponentRemoved(Component* component) {}

	public://RTTI
		template<class Com>
		bool is() { return this->is(std::type_index(typeid(Com))); }
		virtual const std::type_index& getComponentType() = 0;

		virtual uint32_t getComponentTypeID() = 0;
		virtual bool is(const uint32_t& id) = 0;

	protected:
		virtual bool is(const std::type_index& type) = 0;

	protected:
		bool mEnable;

	protected:
		GameObject* mGameObject;

	private:
		static uint32_t sID;
	public:
		static uint32_t giveID()
		{
			return sID++;
		}

		static uint32_t totalID()
		{
			return sID;
		}
	};

	/// <summary>
	/// ComponentT
	/// </summary>
	template<typename Com>
	class TINY_API ComponentT : public Component
	{
	public:
		ComponentT() = default;
		virtual ~ComponentT() = default;

	public:
		const std::type_index& getComponentType() override { return sTypeIndex; }
		bool is(const std::type_index& type) override { return sTypeIndex == type; }

		uint32_t getComponentTypeID() override { return sTypeID; }
		bool is(const uint32_t& id) override { return sTypeID == id; }

	public:
		static const std::type_index& getComponentTypeStatic() { return sTypeIndex; }
		static const uint32_t getComponentTypeIDStatic() { return sTypeID; }

	private:
		static std::type_index sTypeIndex;
		static uint32_t sTypeID;
	};
	template<typename Com>
	uint32_t ComponentT<Com>::sTypeID = Component::giveID();

	template<typename Com>
	std::type_index ComponentT<Com>::sTypeIndex = typeid(Com);
}
