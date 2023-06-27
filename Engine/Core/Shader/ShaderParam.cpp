#include "ShaderParam.h"

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
	UniformID ShaderParam::TexSkybox;
	UniformID ShaderParam::TexIrradiance;
	UniformID ShaderParam::TexPrefilter;
	UniformID ShaderParam::TexBRDFLUT;

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
		ShaderParam::TexSkybox = "TINY_TexSkybox";
		ShaderParam::TexIrradiance = "TINY_TexIrradiance";
		ShaderParam::TexPrefilter = "TINY_TexPrefilter";
		ShaderParam::TexBRDFLUT = "TINY_TexBRDFLUT";

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
