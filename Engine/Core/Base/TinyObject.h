#pragma once
#include "TinyRefObject.h"

namespace tezcat::Tiny
{
	class TINY_API TinyObject : public TinyRefObject
	{
		TINY_ABSTRACT_OBJECT_H(TinyObject, TinyRefObject)
	public:
		TinyObject() {}
		virtual ~TinyObject() = 0;

	public:
		static void destroy(TinyObject* obj);
	};
}
