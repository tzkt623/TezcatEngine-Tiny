#pragma once
#include "Core/Head/ConfigHead.h"
#include "Core/Head/CppHead.h"
#include "StringTool.h"


namespace tezcat::Tiny
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
			if (mIsAttached)
			{
				throw std::logic_error(fmt::format("SG<>: This Instance is an AttachedType [{}]",
					typeid(T).name()));
			}

			mIsAttached = true;
			mInstance = new T(std::forward<Args>(args)...);
		}

		static void destroy()
		{
			if (mIsAttached)
			{
				throw std::logic_error(StringTool::stringFormat("SG<>: This Instance is an AttachedType [%s]",
					typeid(T).name()));
			}

			delete mInstance;
		}

		static void attach(T* instance)
		{
			if (mInstance != nullptr)
			{
				delete instance;
				return;
				throw std::logic_error(StringTool::stringFormat("SG<>: Instance must be null [%s]",
					typeid(T).name()));
			}

			mIsAttached = true;
			mInstance = instance;
		}

		static void detach()
		{
			if (mIsAttached)
			{
				mInstance = nullptr;
			}
			else
			{
				throw std::logic_error(StringTool::stringFormat("SG<>: This Instance is Not an AttachedType [%s]",
					typeid(T).name()));
			}
		}

		static T* getInstance()
		{
			return mInstance;
		}

	private:
		static T* mInstance;
		static bool mIsAttached;
	};

	template<typename T>
	bool SG<T>::mIsAttached = false;

	template<typename T>
	T* SG<T>::mInstance = nullptr;
}
