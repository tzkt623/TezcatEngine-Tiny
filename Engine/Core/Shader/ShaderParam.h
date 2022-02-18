#pragma once

#include "../Head/CppHead.h"
#include "Utility/Tools.h"
#include "Uniform.h"

namespace tezcat::Tiny::Core
{
	//#内置shader参数

	struct TINY_API ShaderParam
	{
		static UniformID create(const char* propertyName);
		static UniformID create(const std::string& propertyName);

		/// <summary>
		/// 解决全局变量初始化不确定的情况
		/// </summary>
		static void initAllParam(const std::function<void()>& customIniter);

		//----------------------------
		//
		// Shader内置参数
		//
	public:
		static UniformID Empty;

		static UniformID MatrixP;
		static UniformID MatrixV;
		static UniformID MatrixM;
		static UniformID MatrixN;

		static UniformID VertexColor;
		static UniformID TexColor;

		static UniformID LightPosition;
		static UniformID LightColor;
		static UniformID ViewPosition;
		static UniformID AmbientStrength;
		static UniformID ModeSpecular;

		struct LightDirection
		{
			static UniformID Direction;
			static UniformID Ambient;
			static UniformID Diffuse;
			static UniformID Specular;
		};

		struct StdMaterial
		{
			static UniformID Diffuse;
			static UniformID Specular;
			static UniformID Shininess;
		};
	};
}