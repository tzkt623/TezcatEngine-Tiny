#pragma once

#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"

namespace tezcat::Tiny::Utility
{
	class TINY_API Tools
	{
	public:
		static std::string fromU8(const std::u8string& string);

		template<typename... Args>
		static std::string stringFormat(const std::string& format, Args... args)
		{
			auto str = format.c_str();
			auto size = 1 + std::snprintf(nullptr, 0, str, args...);
			if (s_StringBuffer.capacity() < size)
			{
				s_StringBuffer.reserve(size);
			}
			std::snprintf(s_StringBuffer.data(), size, str, args...);
			return std::string(s_StringBuffer.data(), size);
		}

		template<typename... Args>
		static int stringFormat(std::string& container, const std::string& format, Args... args)
		{
			auto str = format.c_str();
			auto size = 1 + std::snprintf(nullptr, 0, str, args...);
			container.reserve(size);
			std::snprintf(container.data(), size, str, args...);
			return size;
		}

	private:
		static std::vector<char> s_StringBuffer;
	};


	//-------------------------------
	//
	//  [1/27/2022 Administrator]
	//
	//
	template<class T, typename R>
	class TINY_API IDGenerator
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static R generate()
		{

		}
	};

	template<class T>
	class TINY_API IDGenerator<T, unsigned int>
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static unsigned int generate()
		{
			return m_ID++;
		}

	private:
		static unsigned int m_ID;
	};

	template<class T>
	unsigned int IDGenerator<T, unsigned int>::m_ID = 0;


	template<class T>
	class TINY_API IDGenerator<T, int>
	{
	public:
		IDGenerator() = delete;
		~IDGenerator() = delete;

		static int generate()
		{
			return m_ID++;
		}

	private:
		static int m_ID;
	};

	template<class T>
	int IDGenerator<T, int>::m_ID = 0;

	//--------------------------------------
	//
	// Event
	//
#define BindEvent_0(x) std::bind(&x, this)
#define BindEvent_1(x) std::bind(&x, this, std::placeholders::_1)
#define BindEvent_2(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2)
#define BindEvent_3(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	template<typename... Args>
	class TINY_API Event
	{
		class Data
		{
		public:
			void* master;
			std::function<void(Args...)> callback;
		};
	public:
		Event() { }
		~Event()
		{
			for (auto d : m_List)
			{
				delete d;
			}
		}

	public:
		void addListener(void* master, const std::function<void(Args...)>& function)
		{
			m_List.push_back(new Data{ master, std::move(function) });
		}

		bool removeListener(void* master)
		{
			auto result = std::find(m_List.begin(), m_List.end(),
				[master](Data* data)
				{
					return data->master == master;
				});
			if (result != m_List.end())
			{
				m_List.erase(result);
				return true;
			}

			return false;
		}

		void dispatch(Args... args)
		{
			for (auto d : m_List)
			{
				d->callback(args...);
			}
		}

	private:
		std::vector<Data*> m_List;
	};


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
			ContainerT(T&& other) : m_Value(std::forward<T>(other)) { }

			ContainerT(const T& other) : m_Value(other) { }

			Container* clone() const final
			{
				return new ContainerT<T>(m_Value);
			}

			T m_Value;

			ContainerT& operator=(const ContainerT&) = delete;
		};
	public:
		Any() : m_TypeIndex(typeid(void)), m_Container(nullptr)
		{
		}

		Any(const Any& other) : m_TypeIndex(other.m_TypeIndex), m_Container(other.clone())
		{
		}

		Any(Any&& other) noexcept : m_TypeIndex(other.m_TypeIndex), m_Container(other.m_Container)
		{
			other.m_Container = nullptr;
		}

		template<typename T>
		Any(T& value) : m_TypeIndex(typeid(T)), m_Container(new ContainerT<T>(value))
		{
		}

		template<typename T>
		Any(T&& value) : m_TypeIndex(typeid(T)), m_Container(new ContainerT<T>(std::forward<T>(value)))
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
			m_Container = other.m_Container;
			m_TypeIndex = other.m_TypeIndex;

			other.m_Container = nullptr;
			return *this;
		}

		template <class T>
		Any& operator=(T& value)
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

	//--------------------------------
	//
	// Singleton
	//
	template<typename T>
	class TINY_API SG
	{
	private:
		SG() = delete;
		SG(const SG&) = delete;
		~SG() = delete;

	public:
		template<typename... Args>
		static void init(Args&&... args)
		{
			if (m_IsAttached)
			{
				throw std::logic_error(Tools::stringFormat("SG<>: This Instance is an AttachedType [%s]",
					typeid(T).name()));
			}

			m_Instance = new T(std::forward<Args>(args)...);
		}

		static void destroy()
		{
			if (m_IsAttached)
			{
				throw std::logic_error(Tools::stringFormat("SG<>: This Instance is an AttachedType [%s]",
					typeid(T).name()));
			}

			delete m_Instance;
		}

		static void attach(T* instance)
		{
			if (m_Instance != nullptr)
			{
				throw std::logic_error(Tools::stringFormat("SG<>: Instance must be null [%s]",
					typeid(T).name()));
			}

			m_IsAttached = true;
			m_Instance = instance;
		}

		static void detach()
		{
			if (m_IsAttached)
			{
				m_Instance = nullptr;
			}
			else
			{
				throw std::logic_error(Tools::stringFormat("SG<>: This Instance is Not an AttachedType [%s]",
					typeid(T).name()));
			}
		}

		static T* getInstance()
		{
			return m_Instance;
		}

	private:
		static T* m_Instance;
		static bool m_IsAttached;
	};

	template<typename T>
	bool SG<T>::m_IsAttached = false;

	template<typename T>
	T* SG<T>::m_Instance = nullptr;
}

//---------------------------------------------------
//
// 
//	[2/6/2022 Administrator]
//
//
namespace tezcat::Tiny {}
namespace tezcat::Tiny::Core {}
namespace tezcat::Tiny::Utility {}


using namespace tezcat::Tiny;
using namespace tezcat::Tiny::Core;
using namespace tezcat::Tiny::Utility;
