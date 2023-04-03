#pragma once

#include "../Head/CppHead.h"
#include "TinySmartPointer.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	//---------------------------------------------------
	class TINY_API TinyRef
	{
	public:
		inline void retain()
		{
			m_Ref++;
		}

		inline bool release()
		{
			m_Ref--;
			return m_Ref <= 0;
		}
	private:
		size_t m_Ref;
	};


	//---------------------------------------------------
	class TINY_API TinyObject
	{
		friend class TinyBaseSmartPointer;
	public:
		TinyObject();
		virtual ~TinyObject();

	private:
		TinyRef* m_TinyRef;
	};


	//---------------------------------------------------
	class TINY_API TinyBaseSmartPointer
	{
	public:
		TinyBaseSmartPointer() = default;
		virtual ~TinyBaseSmartPointer() = default;
		virtual TinyObject* getTinyObject() const = 0;

	protected:
	};

	template<typename T, typename = std::enable_if_t<std::is_base_of_v<TinyObject, T>>>
	class TINY_API TinySmartPointer : public TinyBaseSmartPointer
	{
	public:
		TinySmartPointer(T* to)
			: TinyBaseSmartPointer(to)
			, m_TinyPointer(to)
		{
// 			m_Ref = m_TinyRef->;
// 			m_Ref->retain();
		}

		TinySmartPointer(const TinySmartPointer& other)
			: TinyBaseSmartPointer(other.m_TinyPointer)
			, m_TinyPointer(other.m_TinyPointer)
		{

		}

		~TinySmartPointer()
		{
			if (m_Ref->release())
			{
				delete m_TinyPointer;
				delete m_Ref;
			}

			m_TinyPointer = nullptr;
			m_Ref = nullptr;
		}

		TinySmartPointer& operator = (const TinySmartPointer& other)
		{
			m_TinyPointer = other->m_TinyPointer;
			this->holdRef(other);
			return *this;
		}

	public:
		TinyObject* getTinyObject() const final { return m_TinyPointer; }

		T* operator ->() const { return m_TinyPointer; }
		T* get() const { return m_TinyPointer; }

	private:
		T* m_TinyPointer;
		TinyRef* m_Ref;
	};
}
