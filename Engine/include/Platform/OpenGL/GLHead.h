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
#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"
#include "Core/Renderer/RenderConfig.h"
#include "GLGraphics.h"


#ifndef TINY_RELEASE
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
constexpr const char* GLCheckerFMT = "=======GL Error=======\n{}\n{}\n{}\n==================\n";
constexpr const char* GLCheckerFMTPre = "=======GL Pre Error=======\n{}\n{}\n{}\n==================\n";

#define TINY_GL_ERROR(func)\
{\
	auto __tiny_gl_error = glGetError();\
	switch (__tiny_gl_error)\
	{\
	case GL_INVALID_ENUM:\
		fmt::print(GLCheckerFMT, #func, "GL_INVALID_ENUM", __FILE__, __LINE__);\
		__debugbreak();\
		break;\
	case GL_INVALID_VALUE:\
		fmt::print(GLCheckerFMT, #func, "GL_INVALID_VALUE", __FILE__, __LINE__);\
		__debugbreak();\
		break;\
	case GL_INVALID_OPERATION:\
		fmt::print(GLCheckerFMT, #func, "GL_INVALID_OPERATION", __FILE__, __LINE__);\
		__debugbreak();\
		break;\
	case GL_NO_ERROR:\
		break;\
	}\
}

#define TINY_GL_ERROR_PRE(error, func)\
{\
	switch (error)\
	{\
	case GL_INVALID_ENUM:\
		fmt::print(GLCheckerFMTPre, #func, "GL_INVALID_ENUM", __FILE__, __LINE__);\
		__debugbreak();\
		break;\
	case GL_INVALID_VALUE:\
		fmt::print(GLCheckerFMTPre, #func, "GL_INVALID_VALUE", __FILE__, __LINE__);\
		__debugbreak();\
		break;\
	case GL_INVALID_OPERATION:\
		fmt::print(GLCheckerFMTPre, #func, "GL_INVALID_OPERATION", __FILE__, __LINE__);\
		__debugbreak();\
		break;\
	case GL_NO_ERROR:\
		break;\
	}\
}

#define TINY_GL_CLEAR_ERROR(func)\
{\
	while (true)\
	{\
		auto __tiny_gl_error = glGetError();\
		if(__tiny_gl_error == GL_NO_ERROR)\
		{\
			break;\
		}\
		TINY_GL_ERROR_PRE(__tiny_gl_error, func)\
	}\
}

#define TINY_GL_CHECK(x)\
TINY_GL_CLEAR_ERROR(x)\
x;\
TINY_GL_ERROR(x)

#define TINY_GL_CHECK_RETURN(x, result)\
TINY_GL_CLEAR_ERROR(x)\
auto result = x;\
TINY_GL_ERROR(x)
#endif // TINY_GL_TRACKING

#else
#define TINY_GL_CHECK(x) x
#define TINY_GL_CHECK_RETURN(x, result) auto result = x
#endif // !TINY_RELEASE



