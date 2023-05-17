#pragma once

#include "../Head/CppHead.h"
#include "../Tool/Tool.h"
#include "Uniform.h"

namespace tezcat::Tiny
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
		static UniformID MatrixSBV;
		static UniformID MatrixLit;
		static UniformID MatrixEnv;

		static UniformID ViewPosition;
		static UniformID ViewNearFar;
		static UniformID ScreenLength;
		static UniformID IsHDR;

		static UniformID VertexColor;
		static UniformID TexColor;
		static UniformID TexCube;
		static UniformID TexDepth;
		static UniformID TexEnv;

// 		static UniformID LightPosition;
// 		static UniformID LightColor;
//		static UniformID AmbientStrength;
// 		static UniformID ModeSpecular;
//		static UniformID EnableShadow;

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
			static UniformID Normal;
			static UniformID Specular;
			static UniformID Shininess;
		};

		struct MatPBR_Test
		{
			static UniformID Albedo;
			static UniformID Metallic;
			static UniformID Roughness;
			static UniformID AO;
		};

		struct MatPBR_Std1
		{
			static UniformID Albedo2D;
			static UniformID Metallic2D;
			static UniformID Roughness2D;
			static UniformID AO2D;
		};
	};
}
