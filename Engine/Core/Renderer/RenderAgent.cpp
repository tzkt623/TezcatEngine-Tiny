#include "RenderAgent.h"
#include "RenderObject.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Component/Transform.h"
#include "../Data/Material.h"

#include "RenderCommand.h"
#include "RenderPass.h"
#include "BaseGraphics.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(RenderAgent);

	RenderAgent::RenderAgent(IRenderObject* object)
		: mVertex(nullptr)
		, mRenderObject(object)
		, mComponent(nullptr)
	{

	}

	RenderAgent::~RenderAgent()
	{
		mRenderObject = nullptr;
		mComponent = nullptr;
	}

	void RenderAgent::setVertex(Vertex* vertex)
	{
		mVertex = vertex;
	}

	void RenderAgent::sendToQueue(BaseGraphics* graphics, const RenderPhase& phase, RenderQueue* queue)
	{
		mRenderObject->sendToQueue(graphics, phase, queue);
	}

	void RenderAgent::setComponent(Component* component)
	{
		mComponent = component;
	}

}

