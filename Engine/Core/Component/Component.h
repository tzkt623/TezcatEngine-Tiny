#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"


namespace tezcat::Tiny::Core
{
#define LogicBind(func) std::bind(&func, this)

	class GameObject;
	class Transform;
	class TINY_API Component
	{
		friend class GameObject;
	public:
		Component();
		virtual ~Component();

		/// <summary>
		/// ��ʼÿ֡����
		/// </summary>
		void startLogic(const std::function<void()>& logicFunction);

		/// <summary>
		/// ֹͣÿ֡����
		/// </summary>
		void stopLogic();

	protected:
		//���캯������
		virtual void onAwake() {}
		//����Scene����
		virtual void onStart() {}
		//���뵽GameObjectʱ����
		virtual void onEnable()
		{
		}
		//�Ƴ���GameObjectʱ����
		virtual void onDisable()
		{
		}
		//������������
		virtual void onDestroy() {}

		void onDetach()
		{
			m_Enable = false;
			this->onDisable();
			m_GameObject = nullptr;
			m_Transform = nullptr;
		}

	public:
		inline GameObject* getGameObject() const
		{
			return m_GameObject;
		}
		void setGameObject(GameObject* val) { m_GameObject = val; }

		inline Transform* getTransform() const { return m_Transform; }
		void setTransform(Transform* val) { m_Transform = val; }

		bool isEnable() const { return m_Enable; }
		void setEnable(bool val)
		{
			m_Enable = val;
			if (m_Enable)
			{
				this->onEnable();
			}
			else
			{
				this->onDisable();
			}
		}

		bool isDetached() const { return m_GameObject == nullptr; }

	public:
		template<class Com>
		Com* getComponent();

		template<class Com>
		Com* getComponentInChildren();

	public://RTTI
		template<class Com>
		bool is() { return this->is(std::type_index(typeid(Com))); }

		virtual const std::type_index& getComponentType() = 0;

	protected:
		virtual bool is(const std::type_index& type) = 0;

	protected:
		bool m_Enable;

	private:
		GameObject* m_GameObject;
		Transform* m_Transform;
	};

	template<typename Com>
	class TINY_API ComponentT : public Component
	{
	public:
		ComponentT() = default;
		virtual ~ComponentT() = default;

	public:
		const std::type_index& getComponentType() override { return s_TypeIndex; }
		bool is(const std::type_index& type) override { return s_TypeIndex == type; }

	public:
		static const std::type_index& getComponentTypeStatic() { return s_TypeIndex; }

	private:
		static std::type_index s_TypeIndex;
	};

	template<typename Com>
	std::type_index ComponentT<Com>::s_TypeIndex = typeid(Com);
}