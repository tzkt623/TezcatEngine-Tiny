/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderObjectCache.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/RenderCommand.h"

#include "Core/Manager/CameraManager.h"
#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/ShadowCasterManager.h"
#include "Core/Manager/LightingManager.h"
		
#include "Core/Component/Camera.h"
#include "Core/Component/GameObject.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Component/Light.h"
#include "Core/Component/ShadowCaster.h"
		
#include "Core/Data/MeshData.h"
#include "Core/Event/EngineEvent.h"
		
#include "Core/Debug/Debug.h"

#include "Core/Engine.h"



namespace tezcat::Tiny
{
	BaseGraphics::BaseGraphics()
		: mPolygonMode(ContextMap::PolygonModeArray[(uint32_t)PolygonMode::Face])
		, mClearColor(0.0f, 0.0f, 0.0f, 1.0f)
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_ReadObjectID, this
			, [this](const EventData& data)
			{
				int* pos = (int*)data.userData;
				this->addCommand<RenderCMD_ReadObjectID>(pos[0], pos[1], FrameBufferManager::find("FB_ObjectID"));
			});
	}

	BaseGraphics::~BaseGraphics()
	{

	}

	void BaseGraphics::init(Engine* engine)
	{

	}

	void BaseGraphics::buildCommand()
	{
		if (mBuildCommandList.empty())
		{
			return;
		}

		uint32_t index = 0;
		while (index < mBuildCommandList.size())
		{
			//不能引用
			//迭代器会变
			auto cmd = mBuildCommandList[index];
			cmd->execute();
			delete cmd;
			index++;
		}

		mBuildCommandList.clear();
	}

	//---------------------------------------------------------
	//
	//	draw
	//
	void BaseGraphics::drawLine(const float3& begin, const float3& end, const float3& color)
	{

	}

	void BaseGraphics::addCommand(RenderCommadBuild* cmd)
	{
		mBuildCommandList.push_back(cmd);
		//cmd->execute(nullptr);


// 		if (Engine::sMultiThread)
// 		{
// 			mBuildCommandList.push_back(cmd);
// 		}
// 		else
// 		{
// 			cmd->execute(nullptr);
// 		}
	}

	void BaseGraphics::setPolygonMode(PolygonMode mode)
	{
		mPolygonMode = ContextMap::PolygonModeArray[(uint32_t)mode];
	}

	void BaseGraphics::unbind(FrameBuffer* frameBuffer)
	{
		if (mFrameBufferStack.top() != frameBuffer)
		{
			throw std::invalid_argument("Unbind FrameBuffer Error!!! the buffer must be the same");
		}

		mFrameBufferStack.top()->deleteObject();
		mFrameBufferStack.pop();

		if (mFrameBufferStack.empty())
		{
			this->bindImpl(FrameBufferManager::getDefaultBuffer());
			return;
		}

		this->bindImpl(mFrameBufferStack.top());
	}

	void BaseGraphics::bind(FrameBuffer* frameBuffer)
	{
		frameBuffer->saveObject();
		mFrameBufferStack.push(frameBuffer);
		this->bindImpl(frameBuffer);
	}
}
