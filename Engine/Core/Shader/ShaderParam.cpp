#include "ShaderParam.h"

namespace tezcat::Tiny
{
	UniformID ShaderParam::Empty;
	UniformID ShaderParam::MatrixP;
	UniformID ShaderParam::MatrixV;
	UniformID ShaderParam::MatrixM;
	UniformID ShaderParam::MatrixN;
	UniformID ShaderParam::MatrixSBV;
	UniformID ShaderParam::MatrixLit;
	UniformID ShaderParam::MatrixEnv;

	UniformID ShaderParam::ScreenLength;
	UniformID ShaderParam::ViewPosition;
	UniformID ShaderParam::ViewNearFar;
	UniformID ShaderParam::IsHDR;

	UniformID ShaderParam::VertexColor;
	UniformID ShaderParam::TexColor;
	UniformID ShaderParam::TexCube;
	UniformID ShaderParam::TexDepth;
	UniformID ShaderParam::TexEnv;

// 	UniformID ShaderParam::LightPosition;
// 	UniformID ShaderParam::LightColor;
// 	UniformID ShaderParam::ModeSpecular;
// 	UniformID ShaderParam::AmbientStrength;
// 	UniformID ShaderParam::EnableShadow;

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

	UniformID ShaderParam::MatPBR_Test::Albedo;
	UniformID ShaderParam::MatPBR_Test::Metallic;
	UniformID ShaderParam::MatPBR_Test::Roughness;
	UniformID ShaderParam::MatPBR_Test::AO;

	UniformID ShaderParam::MatPBR_Std1::Albedo2D;
	UniformID ShaderParam::MatPBR_Std1::Metallic2D;
	UniformID ShaderParam::MatPBR_Std1::Roughness2D;
	UniformID ShaderParam::MatPBR_Std1::AO2D;

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
		ShaderParam::MatrixLit = "TINY_MatrixLit";
		ShaderParam::MatrixEnv = "TINY_MatrixEnv[6]";

		ShaderParam::ScreenLength = "TINY_ScreenLength";
		ShaderParam::ViewPosition = "TINY_ViewPosition";
		ShaderParam::ViewNearFar = "TINY_ViewNearFar";
		ShaderParam::IsHDR = "TINY_IsHDR";


		//3
		ShaderParam::VertexColor = "TINY_VertexColor";
		ShaderParam::TexColor = "TINY_TexColor";
		ShaderParam::TexCube = "TINY_TexCube";
		ShaderParam::TexDepth = "TINY_TexDepth";
		ShaderParam::TexEnv = "TINY_TexEnv";

		//5
// 		ShaderParam::LightPosition = "TINY_LightPosition";
// 		ShaderParam::LightColor = "TINY_LightColor";
// 		ShaderParam::ModeSpecular = "TINY_ModeSpecular";
// 		ShaderParam::AmbientStrength = "TINY_AmbientStrength";
//		ShaderParam::EnableShadow = "TINY_EnableShadow";

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

		//
		ShaderParam::MatPBR_Test::Albedo = "TINY_MatPBR_Test.albedo";
		ShaderParam::MatPBR_Test::Metallic = "TINY_MatPBR_Test.metallic";
		ShaderParam::MatPBR_Test::Roughness = "TINY_MatPBR_Test.roughness";
		ShaderParam::MatPBR_Test::AO = "TINY_MatPBR_Test.ao";

		//
		ShaderParam::MatPBR_Std1::Albedo2D = "TINY_MatPBR_Std1.albedo2D";
		ShaderParam::MatPBR_Std1::Metallic2D = "TINY_MatPBR_Std1.metallic2D";
		ShaderParam::MatPBR_Std1::Roughness2D = "TINY_MatPBR_Std1.roughness2D";
		ShaderParam::MatPBR_Std1::AO2D = "TINY_MatPBR_Std1.ao2D";

		customIniter();
	}


}
