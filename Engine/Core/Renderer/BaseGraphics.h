#pragma once
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"
#include "../Head/GLMHead.h"

#include "../Shader/Uniform.h"
#include "../Tool/Tool.h"
#include "RenderConfig.h"

namespace tezcat::Tiny
{
	class Vertex;
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

	public:
		void render();
		void setShadowMap(int width, int height);

	public:
		virtual void init(Engine* engine) = 0;

		virtual void preRender();
		virtual void onRender();
		virtual void postRender();
	public:
		virtual void clear(const ClearOption& option) = 0;
		virtual void setViewport(const ViewportInfo& info) = 0;

	public:
		virtual void draw(IRenderMesh* renderMesh) = 0;
		virtual void draw(MeshRenderer* renderer) = 0;
		virtual void draw(Vertex* vertex) = 0;

	public:
		virtual void drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color = glm::vec3(0.0f, 1.0f, 0.0f));
	};

	using Graphics = SG<BaseGraphics>;

//#define Graphics SG<BaseGraphics>::getInstance()
}

