#include "ResourceLoader.h"
#include "../Renderer/BaseGraphics.h"


namespace tezcat::Tiny::Core
{
	ResourceLoader::ResourceLoader(RenderAPI renderAPI)
		: m_WindowWidth(0)
		, m_WindowHeight(0)
	{
		EngineConfig::s_RenderAPI = renderAPI;
	}

	ResourceLoader::~ResourceLoader()
	{

	}

	void ResourceLoader::prepareResource(Engine* engine)
	{
		Graphics->createShaderPackage("../Resource/Shaders/Standard/std2.glsl");
		Graphics->createShaderPackage("../Resource/Shaders/Standard/std1.glsl");
		Graphics->createShaderPackage("../Resource/Shaders/Unlit/color.glsl");
		Graphics->createShaderPackage("../Resource/Shaders/Unlit/texture.glsl");
	}

	void ResourceLoader::prepareEngine(Engine* engine)
	{

	}

	void ResourceLoader::prepareScene(Engine* engine)
	{
	}


}