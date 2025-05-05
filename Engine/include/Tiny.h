#pragma once
/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "Core/Head/TinyCpp.h"

#include "Core/Debug/Debug.h"

#include "Core/Engine.h"
#include "Core/EngineIniter.h"

#include "Core/Base/TinyObject.h"

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/RenderObserver.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/Material.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/RenderObjectCache.h"

#include "Core/Manager/LightingManager.h"
#include "Core/Manager/SceneManager.h"
#include "Core/Manager/CameraManager.h"
#include "Core/Manager/TextureManager.h"
#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Manager/VertexBufferManager.h"
#include "Core/Manager/ModelManager.h"
#include "Core/Manager/ShadowCasterManager.h"
#include "Core/Manager/ResourceManager.h"
#include "Core/Manager/GameObjectManager.h"

#include "Core/Scene.h"

#include "Core/Component/GameObject.h"
#include "Core/Component/Camera.h"
#include "Core/Component/Transform.h"
#include "Core/Component/FlyController.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Component/Light.h"
#include "Core/Component/ShadowCaster.h"

#include "Core/Data/Image.h"
#include "Core/Data/MeshData.h"

#include "Core/Input/Inputer.h"
#include "Core/Input/InputSystem.h"

#include "Core/Shader/ShaderParam.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Shader/Shader.h"
#include "Core/Shader/ShaderParser.h"

#include "Core/Tool/Tool.h"
#include "Core/Event/EngineEvent.h"


#ifdef TINY_PLATFORM_WINDOWS
#include "Platform/OpenGL/GLHead.h"
#endif

using namespace tezcat::Tiny;
using namespace tezcat::Tiny::GL;


#define TINY_ENGINE_ENTRY(my_engine, resouce_loader)\
int main()\
{\
	auto engine = new my_engine();\
	if(engine->init(resouce_loader))\
	{\
		engine->run();\
	}\
	return 0;\
}
