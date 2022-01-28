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

}

