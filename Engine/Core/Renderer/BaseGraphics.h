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
	class Camera;
	struct ViewportInfo;
	class TINY_API BaseGraphics
	{

	public:
		BaseGraphics();
		virtual ~BaseGraphics() = 0;
		void render();

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
		virtual void draw(IRenderObject* renderObject) = 0;
		virtual void draw(MeshRenderer* renderer) = 0;
		virtual void draw(VertexGroup* group, DrawModeWrapper drawMode) = 0;
	};

	using Graphics = SG<BaseGraphics>;
}

