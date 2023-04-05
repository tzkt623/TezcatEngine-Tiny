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
		ShaderBuilderCreator::create("../Resource/Shaders/Unlit/transparent.glsl");

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
		this->createCube();
		this->createSphere();
		this->createSquare();
		this->createSkybox();
	}

	void ResourceLoader::createCube()
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

	void ResourceLoader::createSphere()
	{
		this->createSphere(48);
	}

	void ResourceLoader::createSphere(int prec)
	{
		auto toRadians = [](float degrees)
		{
			return (degrees * 2.0f * 3.14159f) / 360.0f;
		};

		auto mesh_data = new MeshData("Sphere");
		auto vertices_num = (prec + 1) * (prec + 1);
		auto indices_num = prec * prec * 6;

		mesh_data->vertices.resize(vertices_num, glm::vec3());
		mesh_data->uv.resize(vertices_num, glm::vec2());
		mesh_data->normals.resize(vertices_num, glm::vec3());
		mesh_data->colors.resize(vertices_num, glm::vec4(1.0f));
		mesh_data->indices.resize(indices_num, 0);



		for (int i = 0; i <= prec; i++)
		{
			for (int j = 0; j <= prec; j++)
			{
				float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
				float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				float z = (float)sin(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				mesh_data->vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
				mesh_data->uv[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
				//mesh_data->uv[i * (prec + 1) + j] = glm::vec2(0.5f);
				mesh_data->normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
			}
		}


		for (int i = 0; i < prec; i++)
		{
			for (int j = 0; j < prec; j++)
			{
				mesh_data->indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
				mesh_data->indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
				mesh_data->indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
				mesh_data->indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
				mesh_data->indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
				mesh_data->indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
			}
		}

		mesh_data->apply();
	}

	void ResourceLoader::createSquare()
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

	void ResourceLoader::createSkybox()
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
