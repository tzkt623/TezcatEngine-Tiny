﻿#pragma once

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

		std::string getMemoryInfo() override;

	public:
		virtual void onComponentAdded(Component* component) {}
		virtual void onComponentRemoved(Component* component) {}

	public://RTTI
		virtual uint32_t getComponentTypeID() = 0;

	protected:
		virtual void onClose() override;

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


	template<typename Com, uint32_t ID>
	class TINY_API ComponentCustomID : public Component
	{
	protected:
		ComponentCustomID() = default;
	public:
		virtual ~ComponentCustomID() = default;

	public:
		uint32_t getComponentTypeID() override { return sTypeID; }
		bool isComponentID(const uint32_t& id) { return sTypeID == id; }

	public:
		static const uint32_t getComponentTypeIDStatic() { return sTypeID; }

	private:
		static uint32_t sTypeID;
	};

	template<typename Com, uint32_t ID>
	uint32_t ComponentCustomID<Com, ID>::sTypeID = ID;

	/// <summary>
	/// ComponentT
	/// </summary>
	template<typename Com>
	class TINY_API ComponentAutoID : public Component
	{
	protected:
		ComponentAutoID() = default;
	public:
		virtual ~ComponentAutoID() = default;

	public:
		uint32_t getComponentTypeID() override { return sTypeID; }
		bool isComponentID(const uint32_t& id) { return sTypeID == id; }

	public:
		static const uint32_t getComponentTypeIDStatic() { return sTypeID; }

	private:
		static uint32_t sTypeID;
	};

	template<typename Com>
	uint32_t ComponentAutoID<Com>::sTypeID = Component::giveID();
}
