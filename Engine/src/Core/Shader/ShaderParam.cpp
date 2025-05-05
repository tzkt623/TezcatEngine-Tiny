/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "Core/Shader/ShaderParam.h"

namespace tezcat::Tiny
{
	UniformID ShaderParam::Empty;
	UniformID ShaderParam::MatrixP;
	UniformID ShaderParam::MatrixV;
	UniformID ShaderParam::MatrixM;
	UniformID ShaderParam::MatrixN;
	UniformID ShaderParam::MatrixVP;
	UniformID ShaderParam::MatrixMV;
	UniformID ShaderParam::MatrixMVP;
	UniformID ShaderParam::MatrixLightVP;

	UniformID ShaderParam::Resolution;
	UniformID ShaderParam::CameraWorldPosition;
	UniformID ShaderParam::CameraNearFar;
	UniformID ShaderParam::LightWorldPosition;

	UniformID ShaderParam::TexDepth;
	UniformID ShaderParam::TexShadow;
	UniformID ShaderParam::IBL::TexIrradiance;
	UniformID ShaderParam::IBL::TexPrefilter;
	UniformID ShaderParam::IBL::TexBRDFLUT;

	UniformID ShaderParam::SkyBox::ViewIndex;
	UniformID ShaderParam::SkyBox::TexCube;

	UniformID ShaderParam::LightDirection::Direction;
	UniformID ShaderParam::LightDirection::Ambient;
	UniformID ShaderParam::LightDirection::Diffuse;
	UniformID ShaderParam::LightDirection::Specular;

	UniformID ShaderParam::LightPoint::Position;
	UniformID ShaderParam::LightPoint::Ambient;
	UniformID ShaderParam::LightPoint::Diffuse;
	UniformID ShaderParam::LightPoint::Specular;
	UniformID ShaderParam::LightPoint::Config;

	UniformID ShaderParam::create(const char* propertyName)
	{
		return UniformID(propertyName);
	}

	UniformID ShaderParam::create(const std::string& propertyName)
	{
		return UniformID(propertyName);
	}

	void ShaderParam::initAllParam(const std::function<void()>& customIniter)
	{
		//5
		ShaderParam::MatrixP = "TINY_MatrixP";
		ShaderParam::MatrixV = "TINY_MatrixV";
		ShaderParam::MatrixM = "TINY_MatrixM";
		ShaderParam::MatrixMV = "TINY_MatrixMV";
		ShaderParam::MatrixVP = "TINY_MatrixVP";
		ShaderParam::MatrixMVP = "TINY_MatrixMVP";
		ShaderParam::MatrixN = "TINY_MatrixN";
		ShaderParam::MatrixLightVP = "TINY_MatrixLightVP";

		ShaderParam::Resolution = "TINY_Resolution";
		ShaderParam::CameraWorldPosition = "TINY_CameraWorldPosition";
		ShaderParam::CameraNearFar = "TINY_CameraNearFar";
		ShaderParam::LightWorldPosition = "TINY_LightWorldPosition";


		//3
		ShaderParam::TexDepth = "TINY_TexDepth";
		ShaderParam::TexShadow = "TINY_TexShadow";
		ShaderParam::IBL::TexIrradiance = "TINY_TexIrradiance";
		ShaderParam::IBL::TexPrefilter = "TINY_TexPrefilter";
		ShaderParam::IBL::TexBRDFLUT = "TINY_TexBRDFLUT";

		ShaderParam::SkyBox::ViewIndex = "TINY_SkyBox_ViewIndex";
		ShaderParam::SkyBox::TexCube = "TINY_SkyBox_TexCube";

		//4
		ShaderParam::LightDirection::Direction = "TINY_LitDir.direction";
		ShaderParam::LightDirection::Ambient = "TINY_LitDir.ambient";
		ShaderParam::LightDirection::Diffuse = "TINY_LitDir.diffuse";
		ShaderParam::LightDirection::Specular = "TINY_LitDir.specular";

		//5
		ShaderParam::LightPoint::Position = "TINY_LitPoint.position";
		ShaderParam::LightPoint::Ambient = "TINY_LitPoint.ambient";
		ShaderParam::LightPoint::Diffuse = "TINY_LitPoint.diffuse";
		ShaderParam::LightPoint::Specular = "TINY_LitPoint.specular";
		ShaderParam::LightPoint::Config = "TINY_LitPoint.config";

		customIniter();
	}


}
