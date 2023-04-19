#include "MyGUIContext.h"
#include "Window/MyOverviewWindow.h"


MyGUIContext::MyGUIContext()
	: mValueConfigAry(UniformID::allStringCount(), nullptr)
{

	initValueConfig();


}


MyGUIContext::~MyGUIContext()
{

}



void MyGUIContext::selectObject(GameObject* object)
{
	mOverviewWindow->onSelectObject(object);
}

ValueConfig* MyGUIContext::getValueConfig(const UniformID& ID)
{
	return mValueConfigAry[ID.getUID()];
}

void MyGUIContext::initValueConfig()
{
	mValueConfigAry[ShaderParam::LightDirection::Ambient] = new ValueConfig{ 0.005f, 0.0f, 1.0f };
	mValueConfigAry[ShaderParam::LightDirection::Diffuse] = new ValueConfig{ 0.005f, 0.0f, 1.0f };
	mValueConfigAry[ShaderParam::LightDirection::Specular] = new ValueConfig{ 0.005f, 0.0f, 1.0f };


	mValueConfigAry[ShaderParam::StdMaterial::Shininess] = new ValueConfig{ 0.1f, 0.1f, 999.0f };

	mValueConfigAry[ShaderParam::MatPBR_Test::Albedo] = new ValueConfig{ 0.005f, 0.0f, 1.0f };
	mValueConfigAry[ShaderParam::MatPBR_Test::Metallic] = new ValueConfig{ 0.005f, 0.0f, 1.0f };
	mValueConfigAry[ShaderParam::MatPBR_Test::Roughness] = new ValueConfig{ 0.005f, 0.0f, 1.0f };
	mValueConfigAry[ShaderParam::MatPBR_Test::AO] = new ValueConfig{ 0.005f, 0.0f, 1.0f };
}


//-------------------------------------------------------------
//
//
//
//
void MyGUIContext::matrix4(glm::mat4& mat4)
{
	ImGui::InputFloat4("x", glm::value_ptr(mat4[0]));
	ImGui::InputFloat4("y", glm::value_ptr(mat4[1]));
	ImGui::InputFloat4("z", glm::value_ptr(mat4[2]));
	ImGui::InputFloat4("w", glm::value_ptr(mat4[3]));
}

void MyGUIContext::transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	ImGui::DragFloat3("Position", &position.x);
	ImGui::DragFloat3("Rotation", &rotation.x);
	ImGui::DragFloat3("Scale", &scale.x);
}


