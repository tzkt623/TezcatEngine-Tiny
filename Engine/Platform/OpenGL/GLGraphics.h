#pragma once

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Head/ConfigHead.h"

struct GLFWwindow;

namespace tezcat::Tiny::GL
{
	class TINY_API GLGraphics : public BaseGraphics
	{
	public:
		GLGraphics();
		virtual ~GLGraphics();

	public:
		void init(Engine* engine) override;

	public:
		void preRender() override;
		void onRender() override;
		void postRender() override;
		void setViewport(const ViewportInfo& info) override;

		void clear(const ClearOption& option) override;
		void draw(Vertex* vertex) override;
		void drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color = glm::vec3(0.0f, 1.0f, 0.0f)) override;

		void drawLine(Vertex* vertex, const uint32_t& needCount);

	private:
		void initContext();
	};
}
