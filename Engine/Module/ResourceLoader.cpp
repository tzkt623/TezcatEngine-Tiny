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
		auto shader = new Core::Shader("Default", 0);
		shader->registerTextureName(TINY_TexColor);

		shader->attachShader("../Resource/Shaders/vert.glsl", GL_VERTEX_SHADER)
			->attachShader("../Resource/Shaders/frag.glsl", GL_FRAGMENT_SHADER)
			->apply();
	}

	void ResourceLoader::prepareEngine(Core::Engine* engine)
	{

	}

	void ResourceLoader::prepareScene(Core::Engine* engine)
	{
	}
}