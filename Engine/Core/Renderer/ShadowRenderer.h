#pragma once

#include "RenderObject.h"
#include "../Head/ConfigHead.h"
#include "../Head/CppHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class ILight;
	class IRenderMesh;
	class BaseGraphics;
	class RenderPass;
	class FrameBuffer;
	class TINY_API ShadowRenderer
	{

	public:
		static void init(int x, int y, int width, int height);
		static void beginRender();
		static void endRender();
		static void render(BaseGraphics* graphics, ILight* light);
		static void setPass(const std::string&name);
		static void addMesh(IRenderMesh* mesh);

	private:
		static ViewportInfo mViewInfo;
		static FrameBuffer* mFrameBuffer;
		static RenderPass* mPass;
	};
}
