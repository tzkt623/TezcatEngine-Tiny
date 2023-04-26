#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"

namespace tezcat::Tiny
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

	/*
	* @Author: HCL
	* @Info: 2023|4|11
	*
	*/
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
		/*
		* @Author: HCL
		* @Info: 2023|4|13
		* 正常渲染
		*/
		void render(BaseGraphics* graphics);


		/*
		* @Author: HCL
		* @Info: 2023|4|13
		* 只渲染mesh
		*/
		void renderMeshOnly(BaseGraphics* graphics);

		bool checkState();


	private:
		Shader* mShader;
		std::vector<IRenderMesh*> mRenderMeshList;

	public:
		static RenderPass* create(Shader* shader);
		static RenderPass* get(Shader* shader);
		static RenderPass* get(const std::string& name);

	private:
		static std::vector<RenderPass*> sPassAry;
		static std::unordered_map<std::string, RenderPass*> sPassDict;
	};
}

