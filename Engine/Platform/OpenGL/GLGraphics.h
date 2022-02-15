#pragma once

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Head/ConfigHead.h"

struct GLFWwindow;

namespace tezcat::Tiny::Core
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
		void updateViewport(Camera* camera) override;

		void renderMesh(MeshRenderer* renderer) override;

	public:
		VertexGroup* createVertexGroup(MeshData* mesh) override;
		VertexBuffer* createVertexBuffer(MeshData* mesh) override;
		void createShaderPackage(const std::string& filePath) override;
		Texture* createTexture(const std::string& filePath, TextureType type) override;

	private:
		void initContextMap();

	private:
		GLFWwindow* m_Window;
	};
}