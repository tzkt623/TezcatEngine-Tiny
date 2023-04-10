#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class PassVertexGroup;
	class Camera;
	class MeshRenderer;
	class ILight;
	class BaseGraphics;
	class IRenderObject;
	class IRenderMesh;
	class IRenderObserver;

	/// <summary>
	/// 
	/// </summary>
	class TINY_API RenderPass
	{
	public:
		RenderPass(Shader* shader);
		~RenderPass();

	public:
		int getOrderID() const;
		int getProgramID() const;
		const std::string& getName() const;
		Shader* getShader();

	public:
		void addRenderMesh(IRenderMesh* renderObject);
		void sortRenderObjects(const std::function<bool(IRenderMesh* a, IRenderMesh* b)>& function);
		virtual void render(BaseGraphics* graphics, IRenderObserver* renderObserver);

	private:
		Shader* mShader;
		std::vector<IRenderMesh*> mRenderObjects;
	};
}

