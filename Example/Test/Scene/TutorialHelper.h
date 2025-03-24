#pragma once

#include "include/Tiny.h"

class TutorialHelper
{
public:
	static MeshData* createTriangleMesh()
	{
		MeshData* mesh = MeshData::create();
		mesh->mName = "Triangle";
		mesh->mVertices.emplace_back(-0.5f, -0.5f, 0.0f);	// left
		mesh->mVertices.emplace_back(0.5f, -0.5f, 0.0f);	// right
		mesh->mVertices.emplace_back(0.0f, 0.5f, 0.0f);	// top
		mesh->generate();

		return mesh;
	}

	static MeshData* createRectMesh()
	{
		MeshData* mesh = MeshData::create();
		mesh->mName = "Rect";
		mesh->mVertices.emplace_back(-0.5f, -0.5f, 0.0f);	// left-bottom
		mesh->mVertices.emplace_back(0.5f, -0.5f, 0.0f);	// right-bottom
		mesh->mVertices.emplace_back(0.5f, 0.5f, 0.0f);	// right-top
		mesh->mVertices.emplace_back(-0.5f, 0.5f, 0.0f);	// left-top

		mesh->createIndices();
		mesh->mIndices->push_back(0);
		mesh->mIndices->push_back(1);
		mesh->mIndices->push_back(2);
		mesh->mIndices->push_back(0);
		mesh->mIndices->push_back(2);
		mesh->mIndices->push_back(3);

		mesh->createUVs();
		mesh->mUVs->emplace_back(0.0f, 0.0f);
		mesh->mUVs->emplace_back(1.0f, 0.0f);
		mesh->mUVs->emplace_back(1.0f, 1.0f);
		mesh->mUVs->emplace_back(0.0f, 1.0f);
		mesh->generate();

		return mesh;
	}

	static MeshData* createCubeMesh()
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

		mesh_data->generate();
		return mesh_data;
	}

};
