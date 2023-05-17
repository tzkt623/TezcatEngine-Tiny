#include "RenderAgent.h"
#include "RenderObject.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Component/Transform.h"
#include "../Data/Material.h"

#include "RenderCommand.h"
#include "RenderPass.h"

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

	void RenderAgent::submitModelMatrix(Shader* shader)
	{
		auto& model_mat4 = mComponent->getTransform()->getModelMatrix();
		shader->setMat4(ShaderParam::MatrixM, model_mat4);
		glm::mat3 normal_matrix(model_mat4);
		shader->setMat3(ShaderParam::MatrixN, glm::transpose(glm::inverse(normal_matrix)));
	}

	void RenderAgent::setVertex(Vertex* vertex)
	{
		mVertex = vertex;
	}

	void RenderAgent::sendToQueue(const RenderPhase& phase, RenderQueue* queue)
	{
		mRenderObject->sendToQueue(phase, queue);
	}

	void RenderAgent::setComponent(Component* component)
	{
		mComponent = component;
	}

}

