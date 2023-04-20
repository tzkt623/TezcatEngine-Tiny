#include "MyOverviewWindow.h"


MyOverviewWindow::MyOverviewWindow()
	: GUIWindow("对象总览(Object)")
	, mGameObject(nullptr)
	, mOpen(true)
{
	MyGUIContext::getInstance().setOverviewWindow(this);
}

MyOverviewWindow::~MyOverviewWindow()
{

}

void MyOverviewWindow::onSelectObject(GameObject* object)
{
	mGameObject = object;
}

void MyOverviewWindow::onRender()
{
	if (mGameObject == nullptr)
	{
		return;
	}

	auto transform = mGameObject->getTransform();
	ImGui::Text("Name:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), mGameObject->getName().c_str());

	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Position", glm::value_ptr(transform->getPosition()));
		ImGui::DragFloat3("Rotation", glm::value_ptr(transform->getRotation()));
		ImGui::DragFloat3("Scale", glm::value_ptr(transform->getScale()));
		transform->markDirty();
	}

	auto& components = mGameObject->getCompoents();
	for (auto com : components)
	{
		if (com == transform)
		{
			continue;
		}

		com->getComponentID();

		if (com->is<MeshRenderer>())
		{
			this->drawMeshRenderer(com);
		}
		else if (com->is<DirectionalLight>())
		{
			this->drawDirectionalLight(com);
		}
		else if (com->is<Camera>())
		{
			this->drawCamera(com);
		}
	}
}

void MyOverviewWindow::drawCamera(Component* com)
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		auto camera = static_cast<Camera*>(com);
		auto transform = camera->getTransform();

		ImGui::Text("操作<CTRL> [W A S D] [R:Up] [F:Down]");
		ImGui::InputFloat3("位置(Position)", glm::value_ptr(transform->getPosition()));
		ImGui::InputFloat3("旋转(Rotation)", glm::value_ptr(transform->getRotation()));
		ImGui::InputFloat3("缩放(Scale)", glm::value_ptr(transform->getScale()));
		ImGui::Spacing();

		auto up = transform->getUp();
		auto forward = transform->getForward();
		auto right = transform->getRight();
		ImGui::InputFloat3("上(Up)", glm::value_ptr(up));
		ImGui::InputFloat3("前(Forward)", glm::value_ptr(forward));
		ImGui::InputFloat3("右(Right)", glm::value_ptr(right));
		ImGui::Spacing();

		ImGui::Text("视图矩阵(ViewMatrix)");
		MyGUIContext::matrix4(camera->getViewMatrix());
		ImGui::Spacing();

		ImGui::Text("模型矩阵(ModelMatrix)");
		MyGUIContext::matrix4(camera->getTransform()->getModelMatrix());
	}
}

void MyOverviewWindow::drawDirectionalLight(Component* com)
{
	if (ImGui::CollapsingHeader("DirectionalLight"))
	{
		auto lit = static_cast<DirectionalLight*>(com);

		auto config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Ambient);
		ImGui::DragFloat3(UniformID::getStringStatic(ShaderParam::LightDirection::Ambient).data()
			, glm::value_ptr(lit->getAmbient())
			, config->speed
			, config->min
			, config->max);
		ImGui::Spacing();

		config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Diffuse);
		ImGui::DragFloat3(UniformID::getStringStatic(ShaderParam::LightDirection::Diffuse).data()
			, glm::value_ptr(lit->getDiffuse())
			, config->speed
			, config->min
			, config->max);
		ImGui::Spacing();

		config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Specular);
		ImGui::DragFloat3(UniformID::getStringStatic(ShaderParam::LightDirection::Specular).data()
			, glm::value_ptr(lit->getSpecular())
			, config->speed
			, config->min
			, config->max);
		ImGui::Spacing();
	}
}


void MyOverviewWindow::drawMeshRenderer(Component* com)
{
	if (ImGui::CollapsingHeader("MeshRenderer"))
	{
		auto mr = static_cast<MeshRenderer*>(com);

		auto vertex = mr->getVertex();
		

		auto mt = mr->getMaterial();
		auto& uniforms = mt->getUniforms();
		for (auto uniform : uniforms)
		{
			switch (uniform->getType())
			{
			case UniformType::Float:
			{
				auto f1 = (UniformF1*)uniform;
				auto config = MyGUIContext::getInstance().getValueConfig(f1->ID);
				//ImGui::Text(UniformID::getStringStatic(f1->ID).data());
				ImGui::DragFloat(UniformID::getStringStatic(f1->ID).data()
					, &f1->value
					, config->speed
					, config->min
					, config->max);
				ImGui::Spacing();
				break;
			}
			case UniformType::Float3:
			{
				auto f3 = (UniformF3*)uniform;
				auto config = MyGUIContext::getInstance().getValueConfig(f3->ID);
				//ImGui::Text(UniformID::getStringStatic(f3->ID).data());
				ImGui::DragFloat3(UniformID::getStringStatic(f3->ID).data()
					, glm::value_ptr(f3->value)
					, config->speed
					, config->min
					, config->max);
				ImGui::Spacing();
				break;
			}
			case UniformType::Tex2D:
			{
				auto tex = (UniformTex2D*)uniform;
				ImGui::Text(UniformID::getStringStatic(tex->ID).data());
				ImGui::Image(reinterpret_cast<ImTextureID>(tex->value->getTextureID())
					, ImVec2(100, 100)
					, ImVec2(0, 1)
					, ImVec2(1, 0));
				
// 				ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)tex->value->getTextureID()
// 					, ImVec2(0, 100)
//					, ImVec2(100, 0)
// 					, ImVec2(0, 1)
// 					, ImVec2(1, 0)
//					, IM_COL32_WHITE);
				ImGui::Spacing();
				break;
			}
			default:
				break;
			}
		}
	}
}

void MyOverviewWindow::onUpdate()
{

}
