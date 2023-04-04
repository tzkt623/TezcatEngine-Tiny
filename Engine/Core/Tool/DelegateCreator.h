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
			sCreator = creator;
		}

		static void destroy()
		{
			delete sCreator;
		}

		template<typename... Args>
		static BaseType* create(Args&&... args)
		{
			return sCreator->create(std::forward<Args>(args)...);
		}

	private:
		static CreatorType* sCreator;
	};

	template<typename CreatorType, typename BaseType>
	CreatorType* DelegateCreator<CreatorType, BaseType>::sCreator = nullptr;
}
