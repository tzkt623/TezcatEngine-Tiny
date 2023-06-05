#include "RenderCommand.h"
#include "../Shader/Shader.h"

namespace tezcat::Tiny
{
	RenderCommand::RenderCommand(Shader* shader)
		: mShader(shader)
		, mOrderID(0)
	{

	}

	RenderCommand::~RenderCommand()
	{
		mShader = nullptr;
	}
}
