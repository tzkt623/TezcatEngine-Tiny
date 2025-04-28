/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "Core/EngineIniter.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Manager/VertexBufferManager.h"
#include "Core/Data/MeshData.h"
#include "Core/Manager/ResourceManager.h"


namespace tezcat::Tiny
{
	EngineIniter::EngineIniter(RenderAPI renderAPI)
		: mGLMajor(0)
		, mGLMinor(0)
	{
		EngineConfig::sRenderAPI = renderAPI;
	}

	EngineIniter::~EngineIniter()
	{

	}

	void EngineIniter::prepareResource(Engine* engine)
	{
		ShaderManager::loadShaderFiles("Resource/Shaders/Standard");
		ShaderManager::loadShaderFiles("Resource/Shaders/Unlit");
		ShaderManager::loadShaderFiles("Resource/Shaders/Utility");
		ShaderManager::loadShaderFiles("Resource/Shaders/Hide");

		ResourceManager::loadDefault<Image>("Resource/Image/Tiny/TinyDiffuse.png");
		ResourceManager::loadDefault<Image>("Resource/Image/Tiny/TinyShininess.png");
		ResourceManager::loadDefault<Image>("Resource/Image/Tiny/TinySpecular.png");

		this->createBuildinModules();
	}

	void EngineIniter::prepareEngine(Engine* engine)
	{

	}

	void EngineIniter::prepareGame(Engine* engine)
	{

	}

	void EngineIniter::createBuildinModules()
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
		mesh_data->createNormals();

		float size = 0.5f;
		//
		mesh_data->mVertices.emplace_back(-size, -size, size);	//
		mesh_data->mVertices.emplace_back(size, -size, size);	//
		mesh_data->mVertices.emplace_back(size, size, size);	//
		mesh_data->mVertices.emplace_back(-size, size, size);	//

		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);

		//
		mesh_data->mVertices.emplace_back(-size, size, size);	//
		mesh_data->mVertices.emplace_back(size, size, size);	//
		mesh_data->mVertices.emplace_back(size, size, -size);	//
		mesh_data->mVertices.emplace_back(-size, size, -size);	//

		mesh_data->mNormals->emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals->emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals->emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals->emplace_back(0.0f, 1.0f, 0.0f);

		//
		mesh_data->mVertices.emplace_back(-size, size, -size);
		mesh_data->mVertices.emplace_back(size, size, -size);
		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(-size, -size, -size);

		mesh_data->mNormals->emplace_back(0.0f, 0.0f, -1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, -1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, -1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, -1.0f);

		//
		mesh_data->mVertices.emplace_back(-size, -size, -size);
		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(size, -size, size);
		mesh_data->mVertices.emplace_back(-size, -size, size);

		mesh_data->mNormals->emplace_back(0.0f, -1.0f, 0.0f);
		mesh_data->mNormals->emplace_back(0.0f, -1.0f, 0.0f);
		mesh_data->mNormals->emplace_back(0.0f, -1.0f, 0.0f);
		mesh_data->mNormals->emplace_back(0.0f, -1.0f, 0.0f);

		//
		mesh_data->mVertices.emplace_back(-size, -size, -size);
		mesh_data->mVertices.emplace_back(-size, -size, size);
		mesh_data->mVertices.emplace_back(-size, size, size);
		mesh_data->mVertices.emplace_back(-size, size, -size);

		mesh_data->mNormals->emplace_back(-1.0f, 0.0f, 0.0f);
		mesh_data->mNormals->emplace_back(-1.0f, 0.0f, 0.0f);
		mesh_data->mNormals->emplace_back(-1.0f, 0.0f, 0.0f);
		mesh_data->mNormals->emplace_back(-1.0f, 0.0f, 0.0f);

		//
		mesh_data->mVertices.emplace_back(size, -size, size);
		mesh_data->mVertices.emplace_back(size, -size, -size);
		mesh_data->mVertices.emplace_back(size, size, -size);
		mesh_data->mVertices.emplace_back(size, size, size);

		mesh_data->mNormals->emplace_back(1.0f, 0.0f, 0.0f);
		mesh_data->mNormals->emplace_back(1.0f, 0.0f, 0.0f);
		mesh_data->mNormals->emplace_back(1.0f, 0.0f, 0.0f);
		mesh_data->mNormals->emplace_back(1.0f, 0.0f, 0.0f);


		mesh_data->createIndices();
		mesh_data->createColors();
		mesh_data->createUVs();
		for (int i = 0; i < 6; i++)
		{
			int offset = i * 4;
			mesh_data->mIndices->emplace_back(offset + 0);
			mesh_data->mIndices->emplace_back(offset + 1);
			mesh_data->mIndices->emplace_back(offset + 3);

			mesh_data->mIndices->emplace_back(offset + 1);
			mesh_data->mIndices->emplace_back(offset + 2);
			mesh_data->mIndices->emplace_back(offset + 3);

			mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
			mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
			mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
			mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

			mesh_data->mUVs->emplace_back(0.0f, 0.0f);
			mesh_data->mUVs->emplace_back(1.0f, 0.0f);
			mesh_data->mUVs->emplace_back(1.0f, 1.0f);
			mesh_data->mUVs->emplace_back(0.0f, 1.0f);
		}

		mesh_data->generateTangents();
		mesh_data->generate();

		VertexBufferManager::addMeshData(mesh_data);
	}

	void EngineIniter::createSphere()
	{
		this->createSphere(32);
		//this->createSphere2(64, 64);
	}

	void EngineIniter::createSphere2(uint32_t X_SEGMENTS, uint32_t Y_SEGMENTS)
	{
		const float PI = 3.14159265359f;

		MeshData* mesh_data = MeshData::create("Sphere");
		mesh_data->mNormals = new std::vector<float3>();
		mesh_data->mUVs = new std::vector<float2>();

		auto& positions = mesh_data->mVertices;
		auto& uv = mesh_data->mUVs;
		auto& normals = mesh_data->mNormals;

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
				uv->push_back(glm::vec2(xSegment, ySegment));
				normals->push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		mesh_data->mIndices = new std::vector<uint32_t>();
		auto& indices = mesh_data->mIndices;
		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices->push_back(y * (X_SEGMENTS + 1) + x);
					indices->push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}

			// 这里奇偶分开添加是有道理的，奇偶分开添加，就能首位相连，自己可以拿笔画一画
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices->push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices->push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}

		//indices.clear();
		mesh_data->generate();

		VertexBufferManager::addMeshData(mesh_data);
	}

	void EngineIniter::createSphere(int prec)
	{
		auto toRadians = [](float degrees)
		{
			return (degrees * 2.0f * 3.14159f) / 360.0f;
		};

		MeshData* mesh_data = MeshData::create("Sphere");
		mesh_data->createUVs();
		mesh_data->createNormals();
		mesh_data->createColors();
		mesh_data->createIndices();

		auto vertices_num = (prec + 1) * (prec + 1);
		auto indices_num = prec * prec * 6;

		mesh_data->mVertices.resize(vertices_num, glm::vec3());
		mesh_data->mUVs->resize(vertices_num, glm::vec2());
		mesh_data->mNormals->resize(vertices_num, glm::vec3());
		mesh_data->mColors->resize(vertices_num, glm::vec4(1.0f));

		auto& uvs = *mesh_data->mUVs;
		auto& normals = *mesh_data->mNormals;

		for (int i = 0; i <= prec; i++)
		{
			for (int j = 0; j <= prec; j++)
			{
				float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
				float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				float z = (float)sin(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				mesh_data->mVertices[i * (prec + 1) + j] = glm::vec3(x, y, z) * 0.5f;
				uvs[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
				normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
			}
		}

		mesh_data->mIndices->resize(indices_num, 0);
		auto& indices = *mesh_data->mIndices;
		for (int i = 0; i < prec; i++)
		{
			for (int j = 0; j < prec; j++)
			{
				indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
				indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
				indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
			}
		}

		mesh_data->generateTangents();
		mesh_data->generate();
		VertexBufferManager::addMeshData(mesh_data);
	}

	void EngineIniter::createSquare()
	{
		MeshData* mesh_data = MeshData::create("Square");
		mesh_data->createUVs();
		mesh_data->createNormals();
		mesh_data->createColors();
		mesh_data->createIndices();

		mesh_data->mVertices.emplace_back(-0.5f, -0.5f, 0.0f);
		mesh_data->mVertices.emplace_back(0.5f, -0.5f, 0.0f);
		mesh_data->mVertices.emplace_back(0.5f, 0.5f, 0.0f);
		mesh_data->mVertices.emplace_back(-0.5f, 0.5f, 0.0f);

		mesh_data->mIndices->emplace_back(0);
		mesh_data->mIndices->emplace_back(1);
		mesh_data->mIndices->emplace_back(3);

		mesh_data->mIndices->emplace_back(1);
		mesh_data->mIndices->emplace_back(2);
		mesh_data->mIndices->emplace_back(3);

		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

		mesh_data->mUVs->emplace_back(0.0f, 0.0f);
		mesh_data->mUVs->emplace_back(1.0f, 0.0f);
		mesh_data->mUVs->emplace_back(1.0f, 1.0f);
		mesh_data->mUVs->emplace_back(0.0f, 1.0f);

		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);

		mesh_data->generateTangents();
		mesh_data->generate();

		VertexBufferManager::addMeshData(mesh_data);
	}

	void EngineIniter::createRect()
	{
		MeshData* mesh_data = MeshData::create("Rect");
		mesh_data->createUVs();
		mesh_data->createNormals();
		mesh_data->createColors();
		mesh_data->createIndices();

		mesh_data->mVertices.emplace_back(-1.0f, -1.0f, 0.0f);
		mesh_data->mVertices.emplace_back(1.0f, -1.0f, 0.0f);
		mesh_data->mVertices.emplace_back(1.0f, 1.0f, 0.0f);
		mesh_data->mVertices.emplace_back(-1.0f, 1.0f, 0.0f);

		mesh_data->mIndices->emplace_back(0);
		mesh_data->mIndices->emplace_back(1);
		mesh_data->mIndices->emplace_back(3);

		mesh_data->mIndices->emplace_back(1);
		mesh_data->mIndices->emplace_back(2);
		mesh_data->mIndices->emplace_back(3);

		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

		mesh_data->mUVs->emplace_back(0.0f, 0.0f);
		mesh_data->mUVs->emplace_back(1.0f, 0.0f);
		mesh_data->mUVs->emplace_back(1.0f, 1.0f);
		mesh_data->mUVs->emplace_back(0.0f, 1.0f);

		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);
		mesh_data->mNormals->emplace_back(0.0f, 0.0f, 1.0f);

		mesh_data->generateTangents();
		mesh_data->generate();

		VertexBufferManager::addMeshData(mesh_data);
	}

	void EngineIniter::createGridSquare()
	{
		MeshData* mesh_data = MeshData::create("GridSquare");
		mesh_data->mIndices = new std::vector<uint32_t>();

		mesh_data->mVertices.emplace_back(-1.0f, -1.0f, 0.0f);	//
		mesh_data->mVertices.emplace_back(1.0f, -1.0f, 0.0f);	//
		mesh_data->mVertices.emplace_back(1.0f, 1.0f, 0.0f);		//
		mesh_data->mVertices.emplace_back(-1.0f, 1.0f, 0.0f);	//

		mesh_data->mIndices->emplace_back(0);
		mesh_data->mIndices->emplace_back(1);
		mesh_data->mIndices->emplace_back(3);

		mesh_data->mIndices->emplace_back(1);
		mesh_data->mIndices->emplace_back(2);
		mesh_data->mIndices->emplace_back(3);

		mesh_data->generate();
		VertexBufferManager::addMeshData(mesh_data);
	}

	void EngineIniter::createPlane()
	{
		MeshData* mesh_data = MeshData::create("Plane");
		mesh_data->createUVs();
		mesh_data->createNormals();
		mesh_data->createColors();
		mesh_data->createIndices();

		float size = 300.0f;

		mesh_data->mVertices.emplace_back(-size, 0.0f, size);	//
		mesh_data->mVertices.emplace_back(size, 0.0f, size);	//
		mesh_data->mVertices.emplace_back(size, 0.0f, -size);	//
		mesh_data->mVertices.emplace_back(-size, 0.0f, -size);	//

		mesh_data->mIndices->emplace_back(0);
		mesh_data->mIndices->emplace_back(1);
		mesh_data->mIndices->emplace_back(3);

		mesh_data->mIndices->emplace_back(1);
		mesh_data->mIndices->emplace_back(2);
		mesh_data->mIndices->emplace_back(3);

		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
		mesh_data->mColors->emplace_back(1.0f, 1.0f, 1.0f, 1.0f);

		mesh_data->mUVs->emplace_back(0.0f, 0.0f);
		mesh_data->mUVs->emplace_back(1.0f, 0.0f);
		mesh_data->mUVs->emplace_back(1.0f, 1.0f);
		mesh_data->mUVs->emplace_back(0.0f, 1.0f);

		mesh_data->mNormals->emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals->emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals->emplace_back(0.0f, 1.0f, 0.0f);
		mesh_data->mNormals->emplace_back(0.0f, 1.0f, 0.0f);

		mesh_data->generateTangents();
		mesh_data->generate();
		VertexBufferManager::addMeshData(mesh_data);
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

		mesh_data->generate();
		VertexBufferManager::addMeshData(mesh_data);
	}
}
