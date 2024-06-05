#include "RenderCommand.h"
#include "../Shader/Shader.h"

namespace tezcat::Tiny
{
	RenderCommand::RenderCommand()
	{

	}

	RenderCommand::RenderCommand(Vertex* vertex, Transform* transform, Material* material)
		: mVertex(vertex)
		, mTransform(transform)
		, mMaterial(material)
	{

	}

	RenderCommand::~RenderCommand()
	{
		mVertex = nullptr;
		mTransform = nullptr;
		mMaterial = nullptr;
	}
}
