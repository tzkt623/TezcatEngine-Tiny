#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"


namespace tezcat::Tiny
{
	class GameObject;
	class Transform;

	class TINY_API Component : public TinyObject
	{
		friend class GameObject;
		TINY_ABSTRACT_OBJECT_H(Component, TinyObject)

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

		//加入GameObject/手动启用
		virtual void onEnable()
		{

		}

		//离开GameObject/手动关闭
		virtual void onDisable()
		{

		}

		//销毁此组件时执行
		virtual void onDestroy()
		{

		}


	public:
		GameObject* getGameObject() const { return mGameObject; }
		void setGameObject(GameObject* val) { mGameObject = val; }

		Transform* getTransform();

		bool isEnable() const { return mEnable; }
		void setEnable(bool val);

	public:
		virtual void onComponentAdded(Component* component) {}
		virtual void onComponentRemoved(Component* component) {}

	public://RTTI
		virtual uint32 getComponentTypeID() = 0;

	private:
		virtual void onClose() override;

	protected:
		bool mEnable;

	protected:
		GameObject* mGameObject;

	private:
		static uint32 sID;
	public:
		static uint32 giveID()
		{
			return sID++;
		}

		static uint32 totalID()
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
	protected:
		ComponentT() = default;
	public:
		virtual ~ComponentT() = default;

	public:
		uint32 getComponentTypeID() override { return sTypeID; }
		bool isComponentID(const uint32& id) { return sTypeID == id; }

	public:
		static const uint32 getComponentTypeIDStatic() { return sTypeID; }

	private:
		static uint32 sTypeID;
	};

	template<typename Com>
	uint32 ComponentT<Com>::sTypeID = Component::giveID();
}
