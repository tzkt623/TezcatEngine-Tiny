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
		static UniformID TexShadow;
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
