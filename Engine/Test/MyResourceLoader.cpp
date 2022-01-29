#include "MyResourceLoader.h"
#include "MyScene.h"

void MyResourceLoader::prepareEngine(Engine* engine)
{
	m_GameName = u8"YesIndeed,���ѽ�Ϩ,Ȼλ������Ӱ.���,������˹���,û��EldenRing��о�����һ���÷��������!!!!!";
	m_WindowWidth = 1600;
	m_WindowHeight = 900;
}



void MyResourceLoader::prepareScene(Engine* engine)
{
	engine->getSceneManager()->prepareScene(new MyScene("MyScene"));
	engine->getSceneManager()->pushScene("MyScene");
}

void MyResourceLoader::prepareResource(Engine* engine)
{
	ResourceLoader::prepareResource(engine);

	auto mesh_data = new MeshData("Cube");
	//ǰ
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.5f);	//����
	mesh_data->vertices.emplace_back(0.5f, -0.5f, 0.5f);	//����
	mesh_data->vertices.emplace_back(0.5f, 0.5f, 0.5f);		//����
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.5f);	//����

	mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);
	mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);
	mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);
	mesh_data->normals.emplace_back(0.0f, 0.0f, 1.0f);

	//��
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.5f);	//����
	mesh_data->vertices.emplace_back(0.5f, 0.5f, 0.5f);		//����
	mesh_data->vertices.emplace_back(0.5f, 0.5f, -0.5f);	//����
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, -0.5f);	//����

	mesh_data->normals.emplace_back(0.0f, 1.0f, 0.0f);
	mesh_data->normals.emplace_back(0.0f, 1.0f, 0.0f);
	mesh_data->normals.emplace_back(0.0f, 1.0f, 0.0f);
	mesh_data->normals.emplace_back(0.0f, 1.0f, 0.0f);

	//��
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, -0.5f);
	mesh_data->vertices.emplace_back(0.5f, 0.5f, -0.5f);
	mesh_data->vertices.emplace_back(0.5f, -0.5f, -0.5f);
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, -0.5f);

	mesh_data->normals.emplace_back(0.0f, 0.0f, -1.0f);
	mesh_data->normals.emplace_back(0.0f, 0.0f, -1.0f);
	mesh_data->normals.emplace_back(0.0f, 0.0f, -1.0f);
	mesh_data->normals.emplace_back(0.0f, 0.0f, -1.0f);

	//��
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, -0.5f);
	mesh_data->vertices.emplace_back(0.5f, -0.5f, -0.5f);
	mesh_data->vertices.emplace_back(0.5f, -0.5f, 0.5f);
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.5f);

	mesh_data->normals.emplace_back(0.0f, -1.0f, 0.0f);
	mesh_data->normals.emplace_back(0.0f, -1.0f, 0.0f);
	mesh_data->normals.emplace_back(0.0f, -1.0f, 0.0f);
	mesh_data->normals.emplace_back(0.0f, -1.0f, 0.0f);

	//��
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, -0.5f);
	mesh_data->vertices.emplace_back(-0.5f, -0.5f, 0.5f);
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, 0.5f);
	mesh_data->vertices.emplace_back(-0.5f, 0.5f, -0.5f);

	mesh_data->normals.emplace_back(-1.0f, 0.0f, 0.0f);
	mesh_data->normals.emplace_back(-1.0f, 0.0f, 0.0f);
	mesh_data->normals.emplace_back(-1.0f, 0.0f, 0.0f);
	mesh_data->normals.emplace_back(-1.0f, 0.0f, 0.0f);

	//��
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

		mesh_data->uv.emplace_back(1.0f, 1.0f);
		mesh_data->uv.emplace_back(1.0f, 0.0f);
		mesh_data->uv.emplace_back(0.0f, 0.0f);
		mesh_data->uv.emplace_back(0.0f, 1.0f);
	}

	mesh_data->apply();
}
