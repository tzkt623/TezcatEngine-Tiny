#include "TinyObject.h"

namespace tezcat::Tiny::Core
{
	TinyObject::TinyObject()
		: m_TinyRef(new TinyRef())
	{

	}


	TinyObject::~TinyObject()
	{
		if (m_TinyRef->release())
		{
			delete m_TinyRef;
		}

		m_TinyRef = nullptr;
	}
}
