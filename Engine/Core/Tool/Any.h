#pragma once
#include "Core/Head/ConfigHead.h"
#include "Core/Head/CppHead.h"

namespace tezcat::Tiny
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
			: mTypeIndex(typeid(void))
			, mContainer(nullptr)
		{
		}

		Any(const Any& other)
			: mTypeIndex(other.mTypeIndex)
			, mContainer(other.clone())
		{
		}

		Any(Any&& other) noexcept
			: mTypeIndex(other.mTypeIndex)
			, mContainer(other.mContainer)
		{
			other.mContainer = nullptr;
		}

		template<typename T>
		Any(T& value)
			: mTypeIndex(typeid(T))
			, mContainer(new ContainerT<T>(value))
		{
		}

		template<typename T>
		Any(T&& value)
			: mTypeIndex(typeid(T))
			, mContainer(new ContainerT<T>(std::forward<T>(value)))
		{
		}

		~Any()
		{
			delete mContainer;
		}

	private:
		Container* clone() const
		{
			if (mContainer != nullptr)
			{
				return mContainer->clone();
			}

			return nullptr;
		}

	public:
		bool empty() { return mContainer == nullptr; }

		template<class U>
		bool is() const
		{
			return mTypeIndex == std::type_index(typeid(U));
		}

		template<class U>
		U& cast()
		{
			if (!this->is<U>())
			{
				throw std::bad_cast();
			}

			return dynamic_cast<ContainerT<U>*>(mContainer)->m_Value;
		}

		Any& operator=(const Any& other)
		{
			if (mContainer == other.mContainer)
			{
				return *this;
			}

			mContainer = mContainer->clone();
			mTypeIndex = other.mTypeIndex;
			return *this;
		}

		Any& operator=(Any&& other) noexcept
		{
			if (mContainer == other.mContainer)
			{
				return *this;
			}

			delete mContainer;

			mContainer = other.mContainer;
			mTypeIndex = other.mTypeIndex;

			other.mContainer = nullptr;
			return *this;
		}

		template <class T>
		Any& operator=(const T& value)
		{
			delete mContainer;

			mContainer = new ContainerT<T>(value);
			mTypeIndex = std::type_index(typeid(T));

			return *this;
		}

		template <class T>
		Any& operator=(T&& value)
		{
			delete mContainer;

			mContainer = new ContainerT<T>(std::forward<T>(value));
			mTypeIndex = std::type_index(typeid(T));

			return *this;
		}

	private:
		std::type_index mTypeIndex;
		Container* mContainer;
	};
}
