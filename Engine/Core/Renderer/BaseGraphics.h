#pragma once
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"
#include "../Shader/Uniform.h"
#include "Utility/Utility.h"

namespace tezcat::Tiny::Core
{
	class VertexGroup;
	class MeshRenderer;
	class Engine;
	class RenderLayer;
	class IRenderObject;
	class IRenderMesh;
	class Camera;
	struct ViewportInfo;
	class TINY_API BaseGraphics
	{

	public:
		BaseGraphics();
		virtual ~BaseGraphics() = 0;
		void render();
		void createShadowRenderer(const std::string& texName, int width, int height);

	public:
		virtual void init(Engine* engine) = 0;
		virtual void swapBuffer() = 0;

		virtual void preRender();
		virtual void onRender();
		virtual void postRender();
	public:
		virtual void clear(Camera* camera) = 0;
		virtual void setViewport(ViewportInfo& info) = 0;

	public:
		virtual void draw(IRenderMesh* renderMesh) = 0;
		virtual void draw(MeshRenderer* renderer) = 0;
		virtual void draw(VertexGroup* group, DrawModeWrapper drawMode) = 0;

		bool bindShadowMap();
		bool isEnableShadow();

		void setShadowMap(int width, int height)
		{
			mShadowMapWidth = width;
			mShadowMapHeight = height;
		}

	private:
		int mShadowMapWidth;
		int mShadowMapHeight;
	};

	using Graphics = SG<BaseGraphics>;

//#define Graphics SG<BaseGraphics>::getInstance()
}

