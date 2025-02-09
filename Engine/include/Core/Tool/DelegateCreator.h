#pragma once
#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"

namespace tezcat::Tiny
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
