#pragma once
#include "Core/Head/ConfigHead.h"
#include "Core/Head/CppHead.h"

namespace tezcat::Tiny::Utility
{
	//---------------------------------------------------------
	//
	// Any
	// 
	//
	class TINY_API Any
	{
		struct Container
		{
			virtual ~Container() {}
			virtual Container* clone() const = 0;
		};

		template<typename T>
		struct ContainerT : Container
		{
			ContainerT(T&& other) noexcept
				: m_Value(std::forward<T>(other))
			{
			}

			ContainerT(const T& other)
				: m_Value(other)
			{
			}

			Container* clone() const final
			{
				return new ContainerT<T>(m_Value);
			}

			T m_Value;

			ContainerT& operator=(const ContainerT&) = delete;
		};
	public:
		Any()
			: m_TypeIndex(typeid(void))
			, m_Container(nullptr)
		{
		}

		Any(const Any& other)
			: m_TypeIndex(other.m_TypeIndex)
			, m_Container(other.clone())
		{
		}

		Any(Any&& other) noexcept
			: m_TypeIndex(other.m_TypeIndex)
			, m_Container(other.m_Container)
		{
			other.m_Container = nullptr;
		}

		template<typename T>
		Any(T& value)
			: m_TypeIndex(typeid(T))
			, m_Container(new ContainerT<T>(value))
		{
		}

		template<typename T>
		Any(T&& value)
			: m_TypeIndex(typeid(T))
			, m_Container(new ContainerT<T>(std::forward<T>(value)))
		{
		}

		~Any()
		{
			delete m_Container;
		}

	private:
		Container* clone() const
		{
			if (m_Container != nullptr)
			{
				return m_Container->clone();
			}

			return nullptr;
		}

	public:
		bool empty() { return m_Container == nullptr; }

		template<class U>
		bool is() const
		{
			return m_TypeIndex == std::type_index(typeid(U));
		}

		template<class U>
		U& cast()
		{
			if (!this->is<U>())
			{
				throw std::bad_cast();
			}

			return dynamic_cast<ContainerT<U>*>(m_Container)->m_Value;
		}

		Any& operator=(const Any& other)
		{
			if (m_Container == other.m_Container)
			{
				return *this;
			}

			m_Container = m_Container->clone();
			m_TypeIndex = other.m_TypeIndex;
			return *this;
		}

		Any& operator=(Any&& other) noexcept
		{
			if (m_Container == other.m_Container)
			{
				return *this;
			}

			delete m_Container;

			m_Container = other.m_Container;
			m_TypeIndex = other.m_TypeIndex;

			other.m_Container = nullptr;
			return *this;
		}

		template <class T>
		Any& operator=(const T& value)
		{
			delete m_Container;

			m_Container = new ContainerT<T>(value);
			m_TypeIndex = std::type_index(typeid(T));

			return *this;
		}

		template <class T>
		Any& operator=(T&& value)
		{
			delete m_Container;

			m_Container = new ContainerT<T>(std::forward<T>(value));
			m_TypeIndex = std::type_index(typeid(T));

			return *this;
		}

	private:
		std::type_index m_TypeIndex;
		Container* m_Container;
	};
}