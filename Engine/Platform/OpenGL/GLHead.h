#pragma once

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"
#include "Core/Head/RenderConfig.h"


#ifndef TINY_Release
#define TINY_GL_Check(x)\
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
#else
#define TINY_GL_Check(x) x;
#endif // !TINY_Release



