#pragma once

#include "../Base/TinyObject.h"
#include "Vertex.h"

namespace tezcat::Tiny
{
	class IRenderObject;
	class Component;
	class RenderAgent : public TinyObject
	{
		RenderAgent(Component* component, IRenderObject* object);
		TINY_Factory(RenderAgent)
		TINY_RTTI_H(RenderAgent)
	public:
		virtual ~RenderAgent();

		IRenderObject* getRenderObject() { return mRenderObject; }
		Component* getComponent() { return mComponent; }

	private:
		TinyWeakRef<Vertex> mVertex;
		IRenderObject* mRenderObject;
		Component* mComponent;
	};
}
