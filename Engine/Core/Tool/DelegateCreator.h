#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	template<typename CreatorType, typename BaseType>
	class TINY_API DelegateCreator
	{
	public:
		static void attach(CreatorType* creator)
		{
			s_Creator = creator;
		}

		static void destroy()
		{
			delete s_Creator;
		}

		template<typename... Args>
		static BaseType* create(Args&&... args)
		{
			return s_Creator->create(std::forward<Args>(args)...);
		}

	private:
		static CreatorType* s_Creator;
	};

	template<typename CreatorType, typename BaseType>
	CreatorType* DelegateCreator<CreatorType, BaseType>::s_Creator = nullptr;
}