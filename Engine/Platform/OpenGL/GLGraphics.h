#pragma once

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Head/ConfigHead.h"

struct GLFWwindow;

using namespace tezcat::Tiny::Core;
namespace tezcat::Tiny::GL
{
	class TINY_API GLGraphics : public BaseGraphics
	{
	public:
		GLGraphics();
		virtual ~GLGraphics();

	public:
		void init(Engine* engine) override;
		void swapBuffer() override;

	public:
		void preRender() override;
		void onRender() override;
		void postRender() override;
		void setViewport(ViewportInfo& info) override;

		void clear(const ClearOption& option) override;
		void draw(IRenderMesh* renderMesh) override;
		void draw(MeshRenderer* renderer) override;
		void draw(VertexGroup* group, DrawModeWrapper drawMode) override;

	private:
		void initContext();

	private:
		GLFWwindow* mWindow;
	};
}
