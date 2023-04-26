#pragma once
#include "TinyRefObject.h"

namespace tezcat::Tiny
{
	class TINY_API TinyObject : public TinyRefObject
	{
	public:
		TinyObject() = default;
		virtual ~TinyObject() = default;
	};
}
