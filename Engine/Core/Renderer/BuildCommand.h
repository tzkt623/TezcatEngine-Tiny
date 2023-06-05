#pragma once

#include "../Head/ConfigHead.h"


namespace tezcat::Tiny
{
	class BaseGraphics;
	class TINY_API BuildCommand
	{
	public:
		BuildCommand();
		virtual ~BuildCommand();

		virtual void execute(BaseGraphics* graphics) = 0;
	};
}
