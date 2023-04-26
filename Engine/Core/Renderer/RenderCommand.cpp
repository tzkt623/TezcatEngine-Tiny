#include "RenderCommand.h"




namespace tezcat::Tiny
{
	RenderCommand::RenderCommand()
		: mDrawMode(ContextMap::DrawModeArray[(uint32_t)DrawMode::Triangles])
		, mMeshData(nullptr)
	{

	}

	RenderCommand::~RenderCommand()
	{

	}

	void RenderCommand::setDrawMode(DrawMode val)
	{
		mDrawMode = ContextMap::DrawModeArray[(uint32_t)val];
	}

}
