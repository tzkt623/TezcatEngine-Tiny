#include "ShaderParam.h"

namespace tezcat::Tiny::Core
{
	UniformID ShaderParam::Empty;
	UniformID ShaderParam::MatrixP;
	UniformID ShaderParam::MatrixV;
	UniformID ShaderParam::MatrixM;
	UniformID ShaderParam::MatrixN;
	UniformID ShaderParam::MatrixSBV;

	UniformID ShaderParam::VertexColor;
	UniformID ShaderParam::TexColor;
	UniformID ShaderParam::TexCube;

	UniformID ShaderParam::LightPosition;
	UniformID ShaderParam::LightColor;
	UniformID ShaderParam::ViewPosition;
	UniformID ShaderParam::ModeSpecular;
	UniformID ShaderParam::AmbientStrength;
	UniformID ShaderParam::EnableShadow;

	UniformID ShaderParam::LightDirection::Direction;
	UniformID ShaderParam::LightDirection::Ambient;
	UniformID ShaderParam::LightDirection::Diffuse;
	UniformID ShaderParam::LightDirection::Specular;

	UniformID ShaderParam::LightPoint::Position;
	UniformID ShaderParam::LightPoint::Ambient;
	UniformID ShaderParam::LightPoint::Diffuse;
	UniformID ShaderParam::LightPoint::Specular;
	UniformID ShaderParam::LightPoint::Config;

	UniformID ShaderParam::StdMaterial::Diffuse;
	UniformID ShaderParam::StdMaterial::Normal;
	UniformID ShaderParam::StdMaterial::Specular;
	UniformID ShaderParam::StdMaterial::Shininess;

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
		ShaderParam::MatrixN = "TINY_MatrixN";
		ShaderParam::MatrixSBV = "TINY_MatrixSBV";

		//3
		ShaderParam::VertexColor = "TINY_VertexColor";
		ShaderParam::TexColor = "TINY_TexColor";
		ShaderParam::TexCube = "TINY_TexCube";

		//6
		ShaderParam::LightPosition = "TINY_LightPosition";
		ShaderParam::LightColor = "TINY_LightColor";
		ShaderParam::ViewPosition = "TINY_ViewPosition";
		ShaderParam::ModeSpecular = "TINY_ModeSpecular";
		ShaderParam::AmbientStrength = "TINY_AmbientStrength";
		ShaderParam::EnableShadow = "TINY_EnableShadow";

		//4
		ShaderParam::LightDirection::Direction = "TINY_LitDir.direction";
		ShaderParam::LightDirection::Ambient = "TINY_LitDir.ambient";
		ShaderParam::LightDirection::Diffuse = "TINY_LitDir.diffuse";
		ShaderParam::LightDirection::Specular = "TINY_LitDir.specular";

		//5
		ShaderParam::LightPoint::Position = "TINY_LightPot.position";
		ShaderParam::LightPoint::Ambient = "TINY_LightPot.ambient";
		ShaderParam::LightPoint::Diffuse = "TINY_LightPot.diffuse";
		ShaderParam::LightPoint::Specular = "TINY_LightPot.specular";
		ShaderParam::LightPoint::Config = "TINY_LightPot.config";

		//4
		ShaderParam::StdMaterial::Diffuse = "TINY_MatStd.diffuse";
		ShaderParam::StdMaterial::Normal = "TINY_MatStd.normal";
		ShaderParam::StdMaterial::Specular = "TINY_MatStd.specular";
		ShaderParam::StdMaterial::Shininess = "TINY_MatStd.shininess";

		customIniter();
	}


}
