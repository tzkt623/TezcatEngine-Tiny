#pragma once

#include "Core/Engine.h"
#include "Core/EngineIniter.h"
#include "Core/Profiler.h"

#include "Core/GUI/GUIWidget.h"

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/RenderObject.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/RenderPass.h"

#include "Core/Manager/EnvironmentLightManager.h"
#include "Core/Manager/SceneManager.h"
#include "Core/Manager/CameraManager.h"
#include "Core/Manager/TextureManager.h"
#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Manager/VertexBufferManager.h"
#include "Core/Manager/ModelManager.h"
#include "Core/Manager/ShadowCasterManager.h"

#include "Core/Scene/Scene.h"

#include "Core/Component/GameObject.h"
#include "Core/Component/Camera.h"
#include "Core/Component/Transform.h"
#include "Core/Component/FlyController.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Component/Light.h"
#include "Core/Component/Skybox.h"
#include "Core/Component/ShadowCaster.h"

#include "Core/Data/Image.h"
#include "Core/Data/MeshData.h"
#include "Core/Data/Material.h"
#include "Core/Data/Resource.h"

#include "Core/Input/Inputer.h"
#include "Core/Input/InputSystem.h"

#include "Core/Shader/ShaderParam.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Shader/Shader.h"

#include "Core/Tool/Tool.h"
#include "Core/Event/EngineEvent.h"


#ifdef TINY_PLATFORM_WINDOWS
#include "Platform/OpenGL/WindowsEngine.h"
#endif


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
