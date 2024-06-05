#include "BaseGraphics.h"
#include "Renderer.h"

#include "RenderObjectCache.h"
#include "BuildCommand.h"
#include "FrameBuffer.h"
#include "Pipeline.h"

#include "../Manager/CameraManager.h"
#include "../Manager/FrameBufferManager.h"
#include "../Manager/ShadowCasterManager.h"
#include "../Manager/LightingManager.h"

#include "../Component/Camera.h"
#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Light.h"
#include "../Component/ShadowCaster.h"

#include "../Data/MeshData.h"
#include "../Event/EngineEvent.h"

#include "../Profiler.h"
#include "../Engine.h"



namespace tezcat::Tiny
{
	BaseGraphics::BaseGraphics()
	{
	}

	BaseGraphics::~BaseGraphics()
	{

	}


	void BaseGraphics::init(Engine* engine)
	{
	}

	void BaseGraphics::buildCMD()
	{
		for (auto cmd : mBuildCmdAry)
		{
			cmd->execute(this);
			delete cmd;
		}
		mBuildCmdAry.clear();
	}

	//---------------------------------------------------------
	//
	//	draw
	//
	void BaseGraphics::drawLine(const float3& begin, const float3& end, const float3& color)
	{

	}
}
