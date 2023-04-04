#pragma once

#include "../Head/CppHead.h"
#include "Utility/Utility.h"
#include "Uniform.h"

namespace tezcat::Tiny::Core
{
	//#内置Shader变量
	struct TINY_API ShaderParam
	{
		static UniformID create(const char* propertyName);
		static UniformID create(const std::string& propertyName);


		//初始化所有变量
		//解决静态变量初始化顺序不确定的情况
		static void initAllParam(const std::function<void()>& customIniter);

		//----------------------------
		//
		// 内置shader变量
		//
	public:
		static UniformID Empty;

		static UniformID MatrixP;
		static UniformID MatrixV;
		static UniformID MatrixM;
		static UniformID MatrixN;

		static UniformID VertexColor;
		static UniformID TexColor;
		static UniformID TexCube;

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

		struct LightPoint
		{
			static UniformID Position;

			static UniformID Ambient;
			static UniformID Diffuse;
			static UniformID Specular;

			static UniformID Config;
		};

		struct StdMaterial
		{
			static UniformID Diffuse;
			static UniformID Specular;
			static UniformID Shininess;
		};
	};
}
