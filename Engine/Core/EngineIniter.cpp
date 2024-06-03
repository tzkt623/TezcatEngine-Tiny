#include "EngineIniter.h"
#include "Renderer/BaseGraphics.h"
#include "Manager/ShaderManager.h"
#include "Manager/VertexBufferManager.h"
#include "Data/MeshData.h"


namespace tezcat::Tiny
{
	EngineIniter::EngineIniter(RenderAPI renderAPI)
		: mWindowWidth(0)
		, mWindowHeight(0)
		, mGLMajor(0)
		, mGLMinor(0)
		, mEnableVsync(true)
	{
		EngineConfig::sRenderAPI = renderAPI;
	}

	EngineIniter::~EngineIniter()
	{

	}

	void EngineIniter::prepareResource(Engine* engine)
	{
		ShaderManager::loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Standard");
		ShaderManager::loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Unlit");
		ShaderManager::loadShaderFiles(FileTool::getRootRelativeResDir() + "/Shaders/Utility");

		this->createSomeMode();
	}

	void EngineIniter::prepareEngine(Engine* engine)
	{

	}

	void EngineIniter::prepareGame(Engine* engine)
	{

	}

	void EngineIniter::createSomeMode()
	{
		this->createCube();
		this->createSphere();
		this->createSquare();
		this->createPlane();
		this->createSkybox();
		this->createGridSquare();
		this->createRect();
	}

	void EngineIniter::createCube()
	{
		MeshData* mesh_data = MeshData::create("Cube");
		float size = 0.5f;

		//
		mesh_data->mVertices.emplace_back(-size, -size, size);	//
		mesh_data->mVertices.emplace_back(size, -size, size);	//
		mesh_data->mVertices.emplace_back(size, size, size);	//
		mesh_data->mVertices.emplace_back(-size, size, size);	//

		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);

		//
		mesh_data->mVertices.emplace_back(-size, size, size);	//
		mesh_data->mVertices.emplace_back(size, size, size);	//
		mesh_data->mVertices.emplace_back(size, size, -size);	//
		mesh_data->mVertices.emplace_back(-size, size, -size);	//

		mesh_data->mNormals.emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals.emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals.emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals.emplace_back(0.0f, 1.0f, 0.0f);

		//
		mesh_data->mVertices.emplace_back(-size, size, -size);
		mesh_data->mVertices.emplace_back(size, size, -size);
		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(-size, -size, -size);

		mesh_data->mNormals.emplace_back(0.0f, 0.0f, -1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, -1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, -1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, -1.0f);

		//
		mesh_data->mVertices.emplace_back(-size, -size, -size);
		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(size, -size, size);
		mesh_data->mVertices.emplace_back(-size, -size, size);

		mesh_data->mNormals.emplace_back(0.0f, -1.0f, 0.0f);
		mesh_data->mNormals.emplace_back(0.0f, -1.0f, 0.0f);
		mesh_data->mNormals.emplace_back(0.0f, -1.0f, 0.0f);
		mesh_data->mNormals.emplace_back(0.0f, -1.0f, 0.0f);

		//
		mesh_data->mVertices.emplace_back(-size, -size, -size);
		mesh_data->mVertices.emplace_back(-size, -size, size);
		mesh_data->mVertices.emplace_back(-size, size, size);
		mesh_data->mVertices.emplace_back(-size, size, -size);

		mesh_data->mNormals.emplace_back(-1.0f, 0.0f, 0.0f);
		mesh_data->mNormals.emplace_back(-1.0f, 0.0f, 0.0f);
		mesh_data->mNormals.emplace_back(-1.0f, 0.0f, 0.0f);
		mesh_data->mNormals.emplace_back(-1.0f, 0.0f, 0.0f);

		//
		mesh_data->mVertices.emplace_back(size, -size, size);
		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(size, size, -size);
		mesh_data->mVertices.emplace_back(size, size, size);

		mesh_data->mNormals.emplace_back(1.0f, 0.0f, 0.0f);
		mesh_data->mNormals.emplace_back(1.0f, 0.0f, 0.0f);
		mesh_data->mNormals.emplace_back(1.0f, 0.0f, 0.0f);
		mesh_data->mNormals.emplace_back(1.0f, 0.0f, 0.0f);

		for (int i = 0; i < 6; i++)
		{
			int offset = i * 4;
			mesh_data->mIndices.emplace_back(offset + 0);
			mesh_data->mIndices.emplace_back(offset + 1);
			mesh_data->mIndices.emplace_back(offset + 3);

			mesh_data->mIndices.emplace_back(offset + 1);
			mesh_data->mIndices.emplace_back(offset + 2);
			mesh_data->mIndices.emplace_back(offset + 3);

			mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
			mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
			mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
			mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

			mesh_data->mUVs.emplace_back(0.0f, 0.0f);
			mesh_data->mUVs.emplace_back(1.0f, 0.0f);
			mesh_data->mUVs.emplace_back(1.0f, 1.0f);
			mesh_data->mUVs.emplace_back(0.0f, 1.0f);
		}

		mesh_data->apply();

		VertexBufferManager::add(mesh_data);
	}

	void EngineIniter::createSphere()
	{
		this->createSphere(48);
		//this->createSphere2(64, 64);
	}

	void EngineIniter::createSphere2(uint32_t X_SEGMENTS, uint32_t Y_SEGMENTS)
	{
		const float PI = 3.14159265359f;

		MeshData* mesh_data = MeshData::create("Sphere");

		auto& positions = mesh_data->mVertices;
		auto& uv = mesh_data->mUVs;
		auto& normals = mesh_data->mNormals;
		auto& indices = mesh_data->mIndices;


		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;

				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}

			// 这里奇偶分开添加是有道理的，奇偶分开添加，就能首位相连，自己可以拿笔画一画
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}

		//indices.clear();
		mesh_data->apply(DrawMode::Triangles_Strip);

		VertexBufferManager::add(mesh_data);
	}

	void EngineIniter::createSphere(int prec)
	{
		auto toRadians = [](float degrees)
		{
			return (degrees * 2.0f * 3.14159f) / 360.0f;
		};

		MeshData* mesh_data = MeshData::create("Sphere");

		auto vertices_num = (prec + 1) * (prec + 1);
		auto indices_num = prec * prec * 6;

		mesh_data->mVertices.resize(vertices_num, glm::vec3());
		mesh_data->mUVs.resize(vertices_num, glm::vec2());
		mesh_data->mNormals.resize(vertices_num, glm::vec3());
		mesh_data->mColors.resize(vertices_num, glm::vec4(1.0f));
		mesh_data->mIndices.resize(indices_num, 0);

		for (int i = 0; i <= prec; i++)
		{
			for (int j = 0; j <= prec; j++)
			{
				float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
				float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				float z = (float)sin(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				mesh_data->mVertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
				mesh_data->mUVs[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
				//mesh_data->uv[i * (prec + 1) + j] = glm::vec2(0.5f);
				mesh_data->mNormals[i * (prec + 1) + j] = glm::vec3(x, y, z);
			}
		}

		for (int i = 0; i < prec; i++)
		{
			for (int j = 0; j < prec; j++)
			{
				mesh_data->mIndices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
				mesh_data->mIndices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
				mesh_data->mIndices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
				mesh_data->mIndices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
				mesh_data->mIndices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
				mesh_data->mIndices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
			}
		}


		mesh_data->apply();
		VertexBufferManager::add(mesh_data);
	}

	void EngineIniter::createSquare()
	{
		MeshData* mesh_data = MeshData::create("Square");

		mesh_data->mVertices.emplace_back(-0.5f, -0.5f, 0.0f);
		mesh_data->mVertices.emplace_back(0.5f, -0.5f, 0.0f);
		mesh_data->mVertices.emplace_back(0.5f, 0.5f, 0.0f);
		mesh_data->mVertices.emplace_back(-0.5f, 0.5f, 0.0f);

		mesh_data->mIndices.emplace_back(0);
		mesh_data->mIndices.emplace_back(1);
		mesh_data->mIndices.emplace_back(3);

		mesh_data->mIndices.emplace_back(1);
		mesh_data->mIndices.emplace_back(2);
		mesh_data->mIndices.emplace_back(3);

		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

		mesh_data->mUVs.emplace_back(0.0f, 0.0f);
		mesh_data->mUVs.emplace_back(1.0f, 0.0f);
		mesh_data->mUVs.emplace_back(1.0f, 1.0f);
		mesh_data->mUVs.emplace_back(0.0f, 1.0f);

		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);

		mesh_data->apply();

		VertexBufferManager::add(mesh_data);
	}

	void EngineIniter::createRect()
	{
		MeshData* mesh_data = MeshData::create("Rect");

		mesh_data->mVertices.emplace_back(-1.0f, -1.0f, 0.0f);
		mesh_data->mVertices.emplace_back(1.0f, -1.0f, 0.0f);
		mesh_data->mVertices.emplace_back(1.0f, 1.0f, 0.0f);
		mesh_data->mVertices.emplace_back(-1.0f, 1.0f, 0.0f);

		mesh_data->mIndices.emplace_back(0);
		mesh_data->mIndices.emplace_back(1);
		mesh_data->mIndices.emplace_back(3);

		mesh_data->mIndices.emplace_back(1);
		mesh_data->mIndices.emplace_back(2);
		mesh_data->mIndices.emplace_back(3);

		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

		mesh_data->mUVs.emplace_back(0.0f, 0.0f);
		mesh_data->mUVs.emplace_back(1.0f, 0.0f);
		mesh_data->mUVs.emplace_back(1.0f, 1.0f);
		mesh_data->mUVs.emplace_back(0.0f, 1.0f);

		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals.emplace_back(0.0f, 0.0f, 1.0f);

		mesh_data->apply();

		VertexBufferManager::add(mesh_data);
	}

	void EngineIniter::createGridSquare()
	{
		MeshData* mesh_data = MeshData::create("GridSquare");

		mesh_data->mVertices.emplace_back(-1.0f, -1.0f, 0.0f);	//
		mesh_data->mVertices.emplace_back(1.0f, -1.0f, 0.0f);	//
		mesh_data->mVertices.emplace_back(1.0f, 1.0f, 0.0f);		//
		mesh_data->mVertices.emplace_back(-1.0f, 1.0f, 0.0f);	//

		mesh_data->mIndices.emplace_back(0);
		mesh_data->mIndices.emplace_back(1);
		mesh_data->mIndices.emplace_back(3);

		mesh_data->mIndices.emplace_back(1);
		mesh_data->mIndices.emplace_back(2);
		mesh_data->mIndices.emplace_back(3);

		mesh_data->apply();
		VertexBufferManager::add(mesh_data);
	}

	void EngineIniter::createPlane()
	{
		MeshData* mesh_data = MeshData::create("Plane");

		mesh_data->mVertices.emplace_back(-300.0f, 0.0f, 300.0f);	//
		mesh_data->mVertices.emplace_back(300.0f, 0.0f, 300.0f);	//
		mesh_data->mVertices.emplace_back(300.0f, 0.0f, -300.0f);		//
		mesh_data->mVertices.emplace_back(-300.0f, 0.0f, -300.0f);	//

		mesh_data->mIndices.emplace_back(0);
		mesh_data->mIndices.emplace_back(1);
		mesh_data->mIndices.emplace_back(3);

		mesh_data->mIndices.emplace_back(1);
		mesh_data->mIndices.emplace_back(2);
		mesh_data->mIndices.emplace_back(3);

		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

		mesh_data->mUVs.emplace_back(0.0f, 0.0f);
		mesh_data->mUVs.emplace_back(1.0f, 0.0f);
		mesh_data->mUVs.emplace_back(1.0f, 1.0f);
		mesh_data->mUVs.emplace_back(0.0f, 1.0f);

		mesh_data->mNormals.emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals.emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals.emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals.emplace_back(0.0f, 1.0f, 0.0f);

		mesh_data->apply();
		VertexBufferManager::add(mesh_data);
	}

	void EngineIniter::createSkybox()
	{
		MeshData* mesh_data = MeshData::create("Skybox");

		float size = 1.0f;

		//一个面,两个三角形,没有index,NDC坐标系
		mesh_data->mVertices.emplace_back(-size, size, -size);
		mesh_data->mVertices.emplace_back(-size, -size, -size);
		mesh_data->mVertices.emplace_back(size, -size, -size);

		mesh_data->mVertices.emplace_back(-size, size, -size);
		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(size, size, -size);

		//
		mesh_data->mVertices.emplace_back(-size, -size, size);
		mesh_data->mVertices.emplace_back(-size, -size, -size);
		mesh_data->mVertices.emplace_back(-size, size, -size);
		mesh_data->mVertices.emplace_back(-size, size, -size);
		mesh_data->mVertices.emplace_back(-size, size, size);
		mesh_data->mVertices.emplace_back(-size, -size, size);

		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(size, -size, size);
		mesh_data->mVertices.emplace_back(size, size, size);
		mesh_data->mVertices.emplace_back(size, size, size);
		mesh_data->mVertices.emplace_back(size, size, -size);
		mesh_data->mVertices.emplace_back(size, -size, -size);

		mesh_data->mVertices.emplace_back(-size, -size, size);
		mesh_data->mVertices.emplace_back(-size, size, size);
		mesh_data->mVertices.emplace_back(size, size, size);
		mesh_data->mVertices.emplace_back(size, size, size);
		mesh_data->mVertices.emplace_back(size, -size, size);
		mesh_data->mVertices.emplace_back(-size, -size, size);

		mesh_data->mVertices.emplace_back(-size, size, -size);
		mesh_data->mVertices.emplace_back(size, size, -size);
		mesh_data->mVertices.emplace_back(size, size, size);
		mesh_data->mVertices.emplace_back(size, size, size);
		mesh_data->mVertices.emplace_back(-size, size, size);
		mesh_data->mVertices.emplace_back(-size, size, -size);

		mesh_data->mVertices.emplace_back(-size, -size, -size);
		mesh_data->mVertices.emplace_back(-size, -size, size);
		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(-size, -size, size);
		mesh_data->mVertices.emplace_back(size, -size, size);

		mesh_data->apply();
		VertexBufferManager::add(mesh_data);
	}
}
