#include "ResourceLoader.h"
#include "../Renderer/BaseGraphics.h"
#include "../Shader/ShaderPackage.h"
#include "../Data/MeshData.h"


namespace tezcat::Tiny::Core
{
	ResourceLoader::ResourceLoader(RenderAPI renderAPI)
		: mWindowWidth(0)
		, mWindowHeight(0)
	{
		EngineConfig::sRenderAPI = renderAPI;
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
		ShaderBuilderCreator::create("../Resource/Shaders/Unlit/skybox.glsl");

		this->createSomeMode();
	}

	void ResourceLoader::prepareEngine(Engine* engine)
	{

	}

	void ResourceLoader::prepareScene(Engine* engine)
	{
	}

	void ResourceLoader::createSomeMode()
	{
		{
			auto mesh_data = new MeshData("Cube");
			//
			mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.5f);	//
			mesh_data->vertices.emplace_back(0.5f, -0.5f, 0.5f);	//
			mesh_data->vertices.emplace_back(0.5f, 0.5f, 0.5f);		//
			mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.5f);	//

			mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);
			mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);
			mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);
			mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);

			//
			mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.5f);	//
			mesh_data->vertices.emplace_back(0.5f, 0.5f, 0.5f);		//
			mesh_data->vertices.emplace_back(0.5f, 0.5f, -0.5f);	//
			mesh_data->vertices.emplace_back(-0.5f, 0.5f, -0.5f);	//

			mesh_data->normals.emplace_back(0.0f, 1.0f, 0.0f);
			mesh_data->normals.emplace_back(0.0f, 1.0f, 0.0f);
			mesh_data->normals.emplace_back(0.0f, 1.0f, 0.0f);
			mesh_data->normals.emplace_back(0.0f, 1.0f, 0.0f);

			//
			mesh_data->vertices.emplace_back(-0.5f, 0.5f, -0.5f);
			mesh_data->vertices.emplace_back(0.5f, 0.5f, -0.5f);
			mesh_data->vertices.emplace_back(0.5f, -0.5f, -0.5f);
			mesh_data->vertices.emplace_back(-0.5f, -0.5f, -0.5f);

			mesh_data->normals.emplace_back(0.0f, 0.0f, -1.0f);
			mesh_data->normals.emplace_back(0.0f, 0.0f, -1.0f);
			mesh_data->normals.emplace_back(0.0f, 0.0f, -1.0f);
			mesh_data->normals.emplace_back(0.0f, 0.0f, -1.0f);

			//
			mesh_data->vertices.emplace_back(-0.5f, -0.5f, -0.5f);
			mesh_data->vertices.emplace_back(0.5f, -0.5f, -0.5f);
			mesh_data->vertices.emplace_back(0.5f, -0.5f, 0.5f);
			mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.5f);

			mesh_data->normals.emplace_back(0.0f, -1.0f, 0.0f);
			mesh_data->normals.emplace_back(0.0f, -1.0f, 0.0f);
			mesh_data->normals.emplace_back(0.0f, -1.0f, 0.0f);
			mesh_data->normals.emplace_back(0.0f, -1.0f, 0.0f);

			//
			mesh_data->vertices.emplace_back(-0.5f, -0.5f, -0.5f);
			mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.5f);
			mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.5f);
			mesh_data->vertices.emplace_back(-0.5f, 0.5f, -0.5f);

			mesh_data->normals.emplace_back(-1.0f, 0.0f, 0.0f);
			mesh_data->normals.emplace_back(-1.0f, 0.0f, 0.0f);
			mesh_data->normals.emplace_back(-1.0f, 0.0f, 0.0f);
			mesh_data->normals.emplace_back(-1.0f, 0.0f, 0.0f);

			//
			mesh_data->vertices.emplace_back(0.5f, -0.5f, 0.5f);
			mesh_data->vertices.emplace_back(0.5f, -0.5f, -0.5f);
			mesh_data->vertices.emplace_back(0.5f, 0.5f, -0.5f);
			mesh_data->vertices.emplace_back(0.5f, 0.5f, 0.5f);

			mesh_data->normals.emplace_back(1.0f, 0.0f, 0.0f);
			mesh_data->normals.emplace_back(1.0f, 0.0f, 0.0f);
			mesh_data->normals.emplace_back(1.0f, 0.0f, 0.0f);
			mesh_data->normals.emplace_back(1.0f, 0.0f, 0.0f);

			for (int i = 0; i < 6; i++)
			{
				int offset = i * 4;
				mesh_data->indices.emplace_back(offset + 0);
				mesh_data->indices.emplace_back(offset + 1);
				mesh_data->indices.emplace_back(offset + 3);

				mesh_data->indices.emplace_back(offset + 1);
				mesh_data->indices.emplace_back(offset + 2);
				mesh_data->indices.emplace_back(offset + 3);

				mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
				mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
				mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
				mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

				mesh_data->uv.emplace_back(0.0f, 0.0f);
				mesh_data->uv.emplace_back(1.0f, 0.0f);
				mesh_data->uv.emplace_back(1.0f, 1.0f);
				mesh_data->uv.emplace_back(0.0f, 1.0f);
			}

			mesh_data->apply();
		}

		{
			auto mesh_data = new MeshData("Square");
			mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.0f);	//
			mesh_data->vertices.emplace_back(0.5f, -0.5f, 0.0f);	//
			mesh_data->vertices.emplace_back(0.5f, 0.5f, 0.0f);		//
			mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.0f);	//

			mesh_data->indices.emplace_back(0);
			mesh_data->indices.emplace_back(1);
			mesh_data->indices.emplace_back(3);

			mesh_data->indices.emplace_back(1);
			mesh_data->indices.emplace_back(2);
			mesh_data->indices.emplace_back(3);

			mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
			mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
			mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
			mesh_data->colors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

			mesh_data->uv.emplace_back(0.0f, 0.0f);
			mesh_data->uv.emplace_back(1.0f, 0.0f);
			mesh_data->uv.emplace_back(1.0f, 1.0f);
			mesh_data->uv.emplace_back(0.0f, 1.0f);

			mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);
			mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);
			mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);
			mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);

			mesh_data->apply();
		}

		{
			auto mesh_data = new MeshData("Skybox");
			//一个面,两个三角形,没有index,NDC坐标系
			mesh_data->vertices.emplace_back(-1.0f, 1.0f, -1.0f);
			mesh_data->vertices.emplace_back(-1.0f, -1.0f, -1.0f);
			mesh_data->vertices.emplace_back(1.0f, -1.0f, -1.0f);
			mesh_data->vertices.emplace_back(1.0f, -1.0f, -1.0f);
			mesh_data->vertices.emplace_back(1.0f, 1.0f, -1.0f);
			mesh_data->vertices.emplace_back(-1.0f, 1.0f, -1.0f);

			mesh_data->vertices.emplace_back(-1.0f, -1.0f, 1.0f);
			mesh_data->vertices.emplace_back(-1.0f, -1.0f, -1.0f);
			mesh_data->vertices.emplace_back(-1.0f, 1.0f, -1.0f);
			mesh_data->vertices.emplace_back(-1.0f, 1.0f, -1.0f);
			mesh_data->vertices.emplace_back(-1.0f, 1.0f, 1.0f);
			mesh_data->vertices.emplace_back(-1.0f, -1.0f, 1.0f);

			mesh_data->vertices.emplace_back(1.0f, -1.0f, -1.0f);
			mesh_data->vertices.emplace_back(1.0f, -1.0f, 1.0f);
			mesh_data->vertices.emplace_back(1.0f, 1.0f, 1.0f);
			mesh_data->vertices.emplace_back(1.0f, 1.0f, 1.0f);
			mesh_data->vertices.emplace_back(1.0f, 1.0f, -1.0f);
			mesh_data->vertices.emplace_back(1.0f, -1.0f, -1.0f);

			mesh_data->vertices.emplace_back(-1.0f, -1.0f, 1.0f);
			mesh_data->vertices.emplace_back(-1.0f, 1.0f, 1.0f);
			mesh_data->vertices.emplace_back(1.0f, 1.0f, 1.0f);
			mesh_data->vertices.emplace_back(1.0f, 1.0f, 1.0f);
			mesh_data->vertices.emplace_back(1.0f, -1.0f, 1.0f);
			mesh_data->vertices.emplace_back(-1.0f, -1.0f, 1.0f);

			mesh_data->vertices.emplace_back(-1.0f, 1.0f, -1.0f);
			mesh_data->vertices.emplace_back(1.0f, 1.0f, -1.0f);
			mesh_data->vertices.emplace_back(1.0f, 1.0f, 1.0f);
			mesh_data->vertices.emplace_back(1.0f, 1.0f, 1.0f);
			mesh_data->vertices.emplace_back(-1.0f, 1.0f, 1.0f);
			mesh_data->vertices.emplace_back(-1.0f, 1.0f, -1.0f);

			mesh_data->vertices.emplace_back(-1.0f, -1.0f, -1.0f);
			mesh_data->vertices.emplace_back(-1.0f, -1.0f, 1.0f);
			mesh_data->vertices.emplace_back(1.0f, -1.0f, -1.0f);
			mesh_data->vertices.emplace_back(1.0f, -1.0f, -1.0f);
			mesh_data->vertices.emplace_back(-1.0f, -1.0f, 1.0f);
			mesh_data->vertices.emplace_back(1.0f, -1.0f, 1.0f);

			mesh_data->apply();
		}
	}
}
