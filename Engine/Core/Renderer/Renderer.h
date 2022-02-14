#pragma once

#include "../Head/CppHead.h"

namespace tezcat::Tiny::Core
{
	/// <summary>
	/// ������ʱû����
	/// ������ʱû����
	/// ������ʱû����
	/// ������ʱû����
	/// ������ʱû����
	/// ������ʱû����
	/// ������ʱû����
	/// ������ʱû����
	/// ������ʱû����
	/// </summary>
	class GUI;
	class Engine;
	class VertexGroupManager;
	class MeshRenderer;
	//------------------------------------------
	//
	//	Rednerer
	//	|
	//	Pass
	//	|
	//	----light->camera
	//	|
	//	VAOPass
	//	|
	//	RenderCommand
	//
	//
	//
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void init(Engine* engine);

	public:
		void addMeshRenderer(MeshRenderer* renderer);
	public:
		void render();

	private:
		std::list<MeshRenderer*> m_MeshRendererList;

		GUI* m_GUI;
		VertexGroupManager* m_VAOManager;
	};
}