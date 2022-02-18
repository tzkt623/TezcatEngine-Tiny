#include "ShaderParam.h"

namespace tezcat::Tiny::Core
{
	UniformID ShaderParam::Empty;
	UniformID ShaderParam::MatrixP;
	UniformID ShaderParam::MatrixV;
	UniformID ShaderParam::MatrixM;
	UniformID ShaderParam::MatrixN;

	UniformID ShaderParam::VertexColor;
	UniformID ShaderParam::TexColor;

	UniformID ShaderParam::LightPosition;
	UniformID ShaderParam::LightColor;
	UniformID ShaderParam::ViewPosition;
	UniformID ShaderParam::ModeSpecular;
	UniformID ShaderParam::AmbientStrength;

	UniformID ShaderParam::LightDirection::Direction;
	UniformID ShaderParam::LightDirection::Ambient;
	UniformID ShaderParam::LightDirection::Diffuse;
	UniformID ShaderParam::LightDirection::Specular;

	UniformID ShaderParam::StdMaterial::Diffuse;
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
		ShaderParam::MatrixP = "TINY_MatrixP";
		ShaderParam::MatrixV = "TINY_MatrixV";
		ShaderParam::MatrixM = "TINY_MatrixM";
		ShaderParam::MatrixN = "TINY_MatrixN";

		ShaderParam::VertexColor = "TINY_VertexColor";
		ShaderParam::TexColor = "TINY_TexColor";

		ShaderParam::LightPosition = "TINY_LightPosition";
		ShaderParam::LightColor = "TINY_LightColor";
		ShaderParam::ViewPosition = "TINY_ViewPosition";
		ShaderParam::ModeSpecular = "TINY_ModeSpecular";
		ShaderParam::AmbientStrength = "TINY_AmbientStrength";

		ShaderParam::LightDirection::Direction = "TINY_LightDir.direction";
		ShaderParam::LightDirection::Ambient = "TINY_LightDir.ambient";
		ShaderParam::LightDirection::Diffuse = "TINY_LightDir.diffuse";
		ShaderParam::LightDirection::Specular = "TINY_LightDir.specular";

		ShaderParam::StdMaterial::Diffuse = "TINY_MatStd.diffuse";
		ShaderParam::StdMaterial::Specular = "TINY_MatStd.specular";
		ShaderParam::StdMaterial::Shininess = "TINY_MatStd.shininess";

		customIniter();
	}


}