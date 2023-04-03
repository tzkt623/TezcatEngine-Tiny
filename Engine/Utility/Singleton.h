#pragma once
#include "Core/Head/ConfigHead.h"
#include "Core/Head/CppHead.h"
#include "StringTool.h"


namespace tezcat::Tiny::Utility
{
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
				throw std::logic_error(StringTool::stringFormat("SG<>: This Instance is an AttachedType [%s]",
					typeid(T).name()));
			}

			m_Instance = new T(std::forward<Args>(args)...);
		}

		static void destroy()
		{
			if (m_IsAttached)
			{
				throw std::logic_error(StringTool::stringFormat("SG<>: This Instance is an AttachedType [%s]",
					typeid(T).name()));
			}

			delete m_Instance;
		}

		static void attach(T* instance)
		{
			if (m_Instance != nullptr)
			{
				throw std::logic_error(StringTool::stringFormat("SG<>: Instance must be null [%s]",
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
				throw std::logic_error(StringTool::stringFormat("SG<>: This Instance is Not an AttachedType [%s]",
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