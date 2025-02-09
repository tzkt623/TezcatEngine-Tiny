#pragma once

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
	class TINY_API GLFunction_Vertex
	{
	public:
		void build(Vertex* vertex);
		void build(VertexBuffer* vertexBuffer);
		void build(IndexBuffer* indexBuffer);
	};

	class TINY_API GLFunction_Texture
	{
	public:
		void build(Texture2D* tex2D);
		void build(TextureRender2D* render2D);
		void build(TextureCube* texCube);

	public:
		void update(Texture2D* tex2D);
	};


	class TINY_API GLFunction_FrameBuffer
	{
	public:
		void build(FrameBuffer* frameBuffer);

	private:
		void attachCube(TextureCube* texCube, int colorIndex, int faceIndex, int level = 0);
		void attachCube(TextureCube* texCube);
		void attachRender(TextureRender2D* tex);
		void attach2D(Texture2D* tex);
		void attach(Texture* tex);

	private:
		uint32_t mColorCount;
	};

	class TINY_API GLFunction_Shader
	{
	public:
		void build(Shader* shader);

	private:
		void build(Shader* shader, uint32_t pid, const char* data, uint32_t shaderType);
	};

	class TINY_API GLFunction_PBR
	{
	public:
		void makeHDR2Cube(Shader* shader
			, Vertex* vertex
			, Texture2D* texHDR
			, TextureCube* skybox);
		void makeEnvIrradiance(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance);
		void makeEnvPrefilter(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32 mipMaxLevel
			, uint32 mipWidth
			, uint32 mipHeight
			, int32 resolution);
	};

	class TINY_API GLFunction
	{
	public:
		static GLFunction_Vertex FuncVertex;
		static GLFunction_FrameBuffer FuncFrameBuffer;
		static GLFunction_Shader FuncShader;
		static GLFunction_Texture FuncTexture;
		static GLFunction_PBR FuncPBR;
	};
}
