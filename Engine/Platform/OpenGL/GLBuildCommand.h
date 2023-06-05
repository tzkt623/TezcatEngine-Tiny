#pragma once

#include "Core/Renderer/BuildCommand.h"
#include "Core/Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class Texture;
	class Texture2D;
	class TextureCube;
	class TextureRender2D;
	class Image;
	class Vertex;
	class VertexBuffer;
	class IndexBuffer;
	class FrameBuffer;
	class Shader;
	struct TextureInfo;
}

namespace tezcat::Tiny::GL
{
#pragma region Create
	class TINY_API GLBuildCMD_CreateVertex : public BuildCommand
	{
	public:
		GLBuildCMD_CreateVertex(Vertex* vertex);
		virtual ~GLBuildCMD_CreateVertex();

		void execute(BaseGraphics* graphcis) override;
	private:
		Vertex* mVertex;
	};

	class TINY_API GLBuildCMD_CreateVertexBuffer : public BuildCommand
	{
	public:
		GLBuildCMD_CreateVertexBuffer(VertexBuffer* vertexBuffer);
		virtual ~GLBuildCMD_CreateVertexBuffer();

		void execute(BaseGraphics* graphcis) override;
	private:
		VertexBuffer* mVertexBuffer;
	};

	class TINY_API GLBuildCMD_CreateIndexBuffer : public BuildCommand
	{
	public:
		GLBuildCMD_CreateIndexBuffer(IndexBuffer* indexBuffer);
		virtual ~GLBuildCMD_CreateIndexBuffer();

		void execute(BaseGraphics* graphcis) override;
	private:
		IndexBuffer* mIndexBuffer;
	};


	class TINY_API GLBuildCMD_CreateTextrue : public BuildCommand
	{
		virtual Texture* getBuildTexture() { return nullptr; }
	};

	class TINY_API GLBuildCMD_CreateTextrue2D : public GLBuildCMD_CreateTextrue
	{
	public:
		GLBuildCMD_CreateTextrue2D(Texture2D* tex);
		virtual ~GLBuildCMD_CreateTextrue2D();

		void execute(BaseGraphics* graphcis) override;

	private:
		Texture2D* mTex;
	};

	class TINY_API GLBuildCMD_CreateTextrueCube : public GLBuildCMD_CreateTextrue
	{
	public:
		GLBuildCMD_CreateTextrueCube(TextureCube* tex);
		virtual ~GLBuildCMD_CreateTextrueCube();

		void execute(BaseGraphics* graphcis) override;

	private:
		TextureCube* mTex;
	};


	class TINY_API GLBuildCMD_CreateRender2D : public GLBuildCMD_CreateTextrue
	{
	public:
		GLBuildCMD_CreateRender2D(TextureRender2D* tex);
		virtual ~GLBuildCMD_CreateRender2D();

		void execute(BaseGraphics* graphcis) override;

	private:
		TextureRender2D* mTex;
	};


	class TINY_API GLBuildCMD_CreateFrameBuffer : public BuildCommand
	{
	public:
		GLBuildCMD_CreateFrameBuffer(FrameBuffer* frameBuffer);
		virtual ~GLBuildCMD_CreateFrameBuffer();

		void execute(BaseGraphics* graphcis) override;

	private:
		void attachCube(TextureCube* tex, int colorIndex, int faceIndex, int level = 0);
		void attachCube(TextureCube* tex);
		void attachRender(TextureRender2D* tex);
		void attach2D(Texture2D* tex);
		void attach(Texture* tex);

	private:
		uint32_t mColorCount;
		FrameBuffer* mFrameBuffer;
	};


	class TINY_API GLBuildCMD_CreateShader : public BuildCommand
	{
	public:
		GLBuildCMD_CreateShader(Shader* shader);
		GLBuildCMD_CreateShader(Shader* shader, std::string& content);
		virtual ~GLBuildCMD_CreateShader();

		void execute(BaseGraphics* graphcis) override;

	private:
		void build(uint32_t pid, const char* data, uint32_t shaderType);

	private:
		Shader* mShader;
		std::string mContent;
	};
#pragma endregion

#pragma region Update
	class TINY_API GLBuildCMD_UpdateTextrue2D : public GLBuildCMD_CreateTextrue
	{
	public:
		GLBuildCMD_UpdateTextrue2D(Texture2D* tex);
		virtual ~GLBuildCMD_UpdateTextrue2D();

		void execute(BaseGraphics* graphcis) override;

	private:
		Texture2D* mTex;
	};
#pragma endregion


#pragma region Delete
	class TINY_API GLBuildCMD_DeleteVAO : public BuildCommand
	{
	public:
		GLBuildCMD_DeleteVAO(uint32_t id)
			: mID(id)
		{
		}
		virtual ~GLBuildCMD_DeleteVAO() {}

		void execute(BaseGraphics* graphcis) override;

	private:
		uint32_t mID;
	};

	class TINY_API GLBuildCMD_DeleteVBO : public BuildCommand
	{
	public:
		GLBuildCMD_DeleteVBO(uint32_t id)
			: mID(id)
		{
		}
		virtual ~GLBuildCMD_DeleteVBO() {}

		void execute(BaseGraphics* graphcis) override;

	private:
		uint32_t mID;
	};

	class TINY_API GLBuildCMD_DeleteIBO : public BuildCommand
	{
	public:
		GLBuildCMD_DeleteIBO(uint32_t id)
			: mID(id)
		{
		}
		virtual ~GLBuildCMD_DeleteIBO() {}

		void execute(BaseGraphics* graphcis) override;

	private:
		uint32_t mID;
	};

	class TINY_API GLBuildCMD_DeleteTexture : public BuildCommand
	{
	public:
		GLBuildCMD_DeleteTexture(uint32_t id)
			: mID(id)
		{
		}
		virtual ~GLBuildCMD_DeleteTexture() {}

		void execute(BaseGraphics* graphcis) override;

	private:
		uint32_t mID;
	};

	class TINY_API GLBuildCMD_DeleteRender2D : public BuildCommand
	{
	public:
		GLBuildCMD_DeleteRender2D(uint32_t id)
			: mID(id)
		{
		}
		virtual ~GLBuildCMD_DeleteRender2D() {}

		void execute(BaseGraphics* graphcis) override;

	private:
		uint32_t mID;
	};

	class TINY_API GLBuildCMD_DeleteFrameBuffer : public BuildCommand
	{
	public:
		GLBuildCMD_DeleteFrameBuffer(uint32_t id)
			: mID(id)
		{
		}
		virtual ~GLBuildCMD_DeleteFrameBuffer() {}

		void execute(BaseGraphics* graphcis) override;

	private:
		uint32_t mID;
	};

	class TINY_API GLBuildCMD_DeleteShader : public BuildCommand
	{
	public:
		GLBuildCMD_DeleteShader(uint32_t id)
			: mID(id)
		{
		}
		virtual ~GLBuildCMD_DeleteShader() {}

		void execute(BaseGraphics* graphcis) override;

	private:
		uint32_t mID;
	};
#pragma endregion
}
