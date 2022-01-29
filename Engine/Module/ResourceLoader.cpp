#include "ResourceLoader.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Shader.h"

#include <GLFW/glfw3.h>

using namespace tezcat::Tiny;
using namespace tezcat::Tiny::Core;

namespace tezcat::Tiny::Module
{
	ResourceLoader::~ResourceLoader()
	{

	}

	void ResourceLoader::prepareResource(Core::Engine* engine)
	{
		ShaderBuilder::create("../Resource/Shaders/Unlit/color.glsl");
		ShaderBuilder::create("../Resource/Shaders/Unlit/texture.glsl");
		ShaderBuilder::create("../Resource/Shaders/Standard/std1.glsl");
	}

	void ResourceLoader::prepareEngine(Core::Engine* engine)
	{

	}

	void ResourceLoader::prepareScene(Core::Engine* engine)
	{
	}
}