#include "RenderAgent.h"
#include "RenderObject.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(RenderAgent)


	RenderAgent::RenderAgent(Component* component, IRenderObject* object)
		: mVertex()
		, mRenderObject(object)
		, mComponent(component)
	{

	}

	RenderAgent::~RenderAgent()
	{
		mRenderObject = nullptr;
		mComponent = nullptr;
	}

}

