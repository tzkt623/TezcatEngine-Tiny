#pragma once
#include "Utility/Tools.h"
#include "../Head/ConfigHead.h"
#include "../Head/ContextMap.h"

namespace tezcat::Tiny::Core
{
	class VertexGroup;
	class VertexBuffer;
	class MeshData;
	class MeshRenderer;
	class Engine;
	class Camera;
	class PipelineGroup;
	class RenderLayer;
	class IRenderObejct;
	class Texture;
	class TINY_API BaseGraphics
	{

	public:
		BaseGraphics();
		virtual ~BaseGraphics() = 0;
		virtual void init(Engine* engine) = 0;
		void render();
		virtual void swapBuffer() = 0;

		virtual void preRender();
		virtual void onRender();
		virtual void postRender();
		virtual void updateViewport(Camera* camera) = 0;
		virtual void cullWithCamera(Camera* camera);

	public:
		virtual void renderMesh(MeshRenderer* renderer) = 0;
		virtual void addRenderObject(IRenderObejct* renderObject);

	public:
		virtual VertexBuffer* createVertexBuffer(MeshData* mesh) = 0;
		virtual VertexGroup* createVertexGroup(MeshData* mesh) = 0;
		virtual void createShaderPackage(const std::string& filePath) = 0;
		virtual Texture* createTexture(const std::string& filePath, TextureType type) = 0;

	public:
		void sendMeshData(uint32_t index, MeshRenderer* meshRenderer);
		void onCameraAdd(Camera* camera);

	private:
		// 每一个相机都有一个渲染管线组
		std::vector<PipelineGroup*> m_PipelineWithCamera;

		// 渲染层
		// 
		// 每个相机可以看到其中1层或几层
		// 然后使用相机进行剔除分组
		// 然后发送到管线组当中进行渲染
		std::vector<RenderLayer*> m_RenderLayerList;
	};

	using GraphicsT = SG<BaseGraphics>;
#define Graphics GraphicsT::getInstance()
}

