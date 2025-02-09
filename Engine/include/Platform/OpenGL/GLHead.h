#pragma once

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"
#include "Core/Head/RenderConfig.h"
#include "GLGraphics.h"


#ifndef TINY_Release
struct GLChecker
{
	static void push(const char* function)
	{
		if (pos > 7)
		{
			pos = 0;
		}
		array[pos++] = function;
	}

	static void dump()
	{
		char begin = pos - 1;
		char count = 8;
		while (count-- > 0)
		{
			fmt::print("{0}:{1}\n", (int)count, array[begin--]);
			if (begin < 0)
			{
				begin = 7;
			}
		}
	}

private:
	static std::array<const char*, 8> array;
	static char pos;
};

//#define TINY_GL_TRACKING

#ifdef TINY_GL_TRACKING
#define TINY_GL_CHECK(x)\
x;\
GLChecker::push(#x);\
{\
	auto __tiny_gl_error = glGetError();\
	switch (__tiny_gl_error)\
	{\
	case GL_INVALID_ENUM :\
		fmt::print("=======GL Error=======\n{}\n{}\n{}\n==================\n",  __FILE__, __LINE__, "GL_INVALID_ENUM");\
		GLChecker::dump();\
		abort();\
		break;\
	case GL_INVALID_VALUE :\
		fmt::print("=======GL Error=======\n{}\n{}\n{}\n==================\n",  __FILE__, __LINE__, "GL_INVALID_VALUE");\
		GLChecker::dump();\
		abort();\
		break;\
	case GL_INVALID_OPERATION :\
		fmt::print("=======GL Error=======\n{}\n{}\n{}\n==================\n",  __FILE__, __LINE__, "GL_INVALID_OPERATION");\
		GLChecker::dump();\
		abort();\
		break;\
	case GL_NO_ERROR :\
		break;\
	}\
}
#else
#define TINY_GL_CHECK(x)\
x;\
{\
	auto __tiny_gl_error = glGetError();\
	switch (__tiny_gl_error)\
	{\
	case GL_INVALID_ENUM :\
		fmt::print("=======GL Error=======\n{}\n{}\n{}\n==================\n",  __FILE__, __LINE__, "GL_INVALID_ENUM");\
		abort();\
		break;\
	case GL_INVALID_VALUE :\
		fmt::print("=======GL Error=======\n{}\n{}\n{}\n==================\n",  __FILE__, __LINE__, "GL_INVALID_VALUE");\
		abort();\
		break;\
	case GL_INVALID_OPERATION :\
		fmt::print("=======GL Error=======\n{}\n{}\n{}\n==================\n",  __FILE__, __LINE__, "GL_INVALID_OPERATION");\
		abort();\
		break;\
	case GL_NO_ERROR :\
		break;\
	}\
}
#endif // TINY_GL_TRACKING



#else
#define TINY_GL_CHECK(x) x;
#endif // !TINY_Release



