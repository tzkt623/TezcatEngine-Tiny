#pragma once

#include "Core/Engine.h"
#include "Core/Statistic.h"

#include "Core/GUI/GUIWidget.h"

#include "Core/Renderer/Texture.h"

#include "Core/Manager/SceneManager.h"
#include "Core/Manager/CameraManager.h"

#include "Core/Scene/Scene.h"
#include "Core/Scene/GameObject.h"

#include "Core/Component/RenderObject.h"
#include "Core/Component/Camera.h"
#include "Core/Component/Transform.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Component/Light.h"

#include "Core/Data/Image.h"
#include "Core/Data/MeshData.h"
#include "Core/Data/Material.h"
#include "Core/Data/ResourceLoader.h"

#include "Core/Input/Inputer.h"
#include "Core/Input/InputSystem.h"

#include "Core/Shader/ShaderParam.h"

#include "Utility/Tools.h"

#ifdef TINY_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsEngine.h"
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