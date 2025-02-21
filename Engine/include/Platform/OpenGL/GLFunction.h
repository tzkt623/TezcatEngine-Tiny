#pragma once
/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "Core/Head/TinyCpp.h"
#include "Core/Head/GLMHead.h"

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
	class UniformBuffer;
	class Shader;
	struct TextureInfo;
}

namespace tezcat::Tiny::GL
{
	class TINY_API GLFunction_Buffer
	{
	public:
		void build(Vertex* vertex);
		void build(VertexBuffer* vertexBuffer);
		void build(IndexBuffer* indexBuffer);
		void build(UniformBuffer* uniformBuffer, int32_t bindingIndex);
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
		GLFunction_PBR();

	public:
		void makeHDR2Cube(Shader* shader
			, Vertex* vertex
			, Texture2D* texHDR
			, TextureCube* skybox) const;

		void makeEnvIrradiance(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* irradiance) const;

		void makeEnvPrefilter(Shader* shader
			, Vertex* vertex
			, TextureCube* skybox
			, TextureCube* prefitler
			, uint32 mipMaxLevel
			, uint32 mipWidth
			, uint32 mipHeight
			, int32 resolution) const;

	private:
		float4x4 mCaptureViews[6];
	};

	class TINY_API GLFunction
	{
	public:
		static GLFunction_Buffer FuncBuffer;
		static GLFunction_FrameBuffer FuncFrameBuffer;
		static GLFunction_Shader FuncShader;
		static GLFunction_Texture FuncTexture;
		static GLFunction_PBR FuncPBR;
	};
}
