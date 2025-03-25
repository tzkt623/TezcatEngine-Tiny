#include "MyGUIContext.h"
#include "Window/MyObjectInfoWindow.h"

namespace tezcat::Editor
{
	const ImVec2 MyGUIContext::UV0(0, 1);
	const ImVec2 MyGUIContext::UV1(1, 0);
	ImVec2 MyGUIContext::sViewPortSize;

	MyGUIContext::MyGUIContext()
		: mValueConfigAry(UniformID::allStringCount(), nullptr)
	{
		this->initValueConfig();
	}


	MyGUIContext::~MyGUIContext()
	{

	}


	ValueConfig* MyGUIContext::getValueConfig(const UniformID& ID)
	{
		return mValueConfigAry[ID.toUID()];
	}

	void MyGUIContext::initValueConfig()
	{
		mValueConfigAry[ShaderParam::LightDirection::Ambient.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
		mValueConfigAry[ShaderParam::LightDirection::Diffuse.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
		mValueConfigAry[ShaderParam::LightDirection::Specular.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
	}


	bool MyGUIContext::sIsDragResource = false;

	//-------------------------------------------------------------
	//
	//
	//
	//
	void MyGUIContext::matrix4(glm::mat4& mat4)
	{
		ImGui::Text("x");
		ImGui::SameLine();
		ImGui::InputFloat4("##x", glm::value_ptr(mat4[0]));

		ImGui::Text("y");
		ImGui::SameLine();
		ImGui::InputFloat4("##y", glm::value_ptr(mat4[1]));

		ImGui::Text("z");
		ImGui::SameLine();
		ImGui::InputFloat4("##z", glm::value_ptr(mat4[2]));

		ImGui::Text("w");
		ImGui::SameLine();
		ImGui::InputFloat4("##w", glm::value_ptr(mat4[3]));
	}

	void MyGUIContext::transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
	{
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::DragFloat3("##Position", &position.x);

		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rotation", &rotation.x);

		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::DragFloat3("##Scale", &scale.x);
	}


}
