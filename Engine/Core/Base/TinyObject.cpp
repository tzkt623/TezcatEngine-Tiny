#include "TinyObject.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(TinyObject, TinyRefObject)

	void TinyObject::destroy(TinyObject* obj)
	{
		obj->mGCInfo->strongRef = 0;
		TinyGC::manage(obj);
	}

	TinyObject::~TinyObject()
	{

	}

}

