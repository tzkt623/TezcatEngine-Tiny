#pragma once

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"

namespace tezcat::Tiny::Core
{
	enum DrawMode
	{
		Points = GL_POINTS,
		Lines = GL_LINES,
		Lines_Loop = GL_LINE_LOOP,
		Lines_Strip = GL_LINE_STRIP,
		Triangles = GL_TRIANGLES,
		Triangles_Strip = GL_TRIANGLE_STRIP,
		Triangles_Fan = GL_TRIANGLE_FAN
	};

	enum TexType : int
	{
		Tex_1D = GL_TEXTURE_1D,
		Tex_2D = GL_TEXTURE_2D,
		Tex_3D = GL_TEXTURE_3D,
		Tex_1DA = GL_TEXTURE_1D_ARRAY,
		Tex_2DA = GL_TEXTURE_2D_ARRAY,
		Tex_Cube = GL_TEXTURE_CUBE_MAP
	};

	enum TexWrap : int
	{
		Tex_REPEAT = GL_REPEAT,
		Tex_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		Tex_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		Tex_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

	enum TexFilter : int
	{
		Tex_NEAREST = GL_NEAREST,
		Tex_LINEAR = GL_LINEAR
	};

	enum TexChannel : int
	{
		Tex_RGB = GL_RGB,
		Tex_RGBA = GL_RGBA
	};

	enum VertexMask : int
	{
		Position = 0,
		Normal,
		Color,
		UV
	};

	enum RenderQueue : int
	{
		Skybox = 0,
		Opaque = 2000,
		Transparent = 4000
	};
}

