#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

	enum VertexMask : int
	{
		Position = 0,
		Normal,
		Color,
		UV
	};
}

