#pragma once

#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	template<class Creator>
	class TINY_API Manager
	{
	public:
		Manager() : mCreator(nullptr)
		{
		}

		virtual ~Manager()
		{
			delete mCreator;
		}

		void initCreator(Creator* creator)
		{
			mCreator = creator;
		}

	protected:
		Creator* mCreator;
	};
}

