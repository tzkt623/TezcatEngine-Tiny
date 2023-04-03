#include "ResourceLoader.h"
#include "../Renderer/BaseGraphics.h"
#include "../Shader/ShaderPackage.h"


namespace tezcat::Tiny::Core
{
	ResourceLoader::ResourceLoader(RenderAPI renderAPI)
		: mWindowWidth(0)
		, mWindowHeight(0)
	{
		EngineConfig::s_RenderAPI = renderAPI;
	}

	ResourceLoader::~ResourceLoader()
	{

	}

	void ResourceLoader::prepareResource(Engine* engine)
	{
		ShaderBuilderCreator::create("../Resource/Shaders/Standard/std2.glsl");
		ShaderBuilderCreator::create("../Resource/Shaders/Standard/std1.glsl");
		ShaderBuilderCreator::create("../Resource/Shaders/Unlit/color.glsl");
		ShaderBuilderCreator::create("../Resource/Shaders/Unlit/texture.glsl");
	}

	void ResourceLoader::prepareEngine(Engine* engine)
	{

	}

	void ResourceLoader::prepareScene(Engine* engine)
	{
	}


}
