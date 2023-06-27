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
		static UniformID MatrixMV;
		static UniformID MatrixMVP;
		static UniformID MatrixVP;
		static UniformID MatrixN;
		static UniformID MatrixLightVP;

		static UniformID CameraWorldPosition;
		static UniformID CameraNearFar;
		static UniformID Resolution;
		static UniformID LightWorldPosition;

		static UniformID TexDepth;
		static UniformID TexSkybox;
		static UniformID TexIrradiance;
		static UniformID TexPrefilter;
		static UniformID TexBRDFLUT;

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
	};
}
