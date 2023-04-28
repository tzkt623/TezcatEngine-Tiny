#pragma once

#include "../Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class RenderCommand;
	class RenderHelper
	{
		RenderHelper() = delete;
		~RenderHelper() = delete;
	public:

		static void renderPreData();
		static void render2D();
		static void renderPostData();


	private:
		static std::list<RenderCommand*> sPreList;
	};
}
