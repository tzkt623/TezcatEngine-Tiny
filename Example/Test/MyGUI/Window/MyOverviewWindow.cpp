#include "MyOverviewWindow.h"
#include "../../MyEvent.h"

#define BindDrawFunction(x) std::bind(&MyOverviewWindow::x, this, std::placeholders::_1)

MyOverviewWindow::MyOverviewWindow()
	: GUIWindow("对象总览(Object)")
	, mGameObject(nullptr)
	, mOpen(true)
	, mDrawFunctions()
{
	MyEvent::get()->addListener(MyEventID::Window_ObjectSelected, this, [this](const EventData& data)
		{
			mGameObject = static_cast<GameObject*>(data.userData);
		});

	this->drawComponent<Transform>([](Component* com)
		{
			if (ImGui::CollapsingHeader("坐标(Transform)"))
			{
				auto transform = static_cast<Transform*>(com);
				ImGui::DragFloat3("Position", glm::value_ptr(transform->getPosition()));
				ImGui::DragFloat3("Rotation", glm::value_ptr(transform->getRotation()));
				ImGui::DragFloat3("Scale", glm::value_ptr(transform->getScale()));
				transform->markDirty();
			}
		});

	this->drawComponent<MeshRenderer>([](Component* com)
		{
			if (ImGui::CollapsingHeader("网格渲染器(MeshRenderer)"))
			{
				auto mr = static_cast<MeshRenderer*>(com);
				ImGui::Text("Mesh");
				auto vertex = mr->getVertex();
				ImGui::Text(StringTool::stringFormat("Name: %s", vertex->getName().c_str()).c_str());
				ImGui::Separator();
				ImGui::Text("Material");
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
						if (config->isColor)
						{
							ImGui::ColorEdit3(UniformID::getStringStatic(f3->ID).data()
								, glm::value_ptr(f3->value));
						}
						else
						{
							ImGui::DragFloat3(UniformID::getStringStatic(f3->ID).data()
								, glm::value_ptr(f3->value)
								, config->speed
								, config->min
								, config->max);
						}

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
						ImGui::Spacing();
						break;
					}
					default:
						break;
					}
				}
			}
		});

	this->drawComponent<DirectionalLight>([](Component* com)
		{
			if (ImGui::CollapsingHeader("区域光(DirectionalLight)"))
			{
				auto lit = static_cast<DirectionalLight*>(com);

				auto config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Ambient);
				ImGui::ColorEdit3(UniformID::getStringStatic(ShaderParam::LightDirection::Ambient).data()
					, glm::value_ptr(lit->getAmbient()));
				ImGui::Spacing();

				config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Diffuse);
				ImGui::ColorEdit3(UniformID::getStringStatic(ShaderParam::LightDirection::Diffuse).data()
					, glm::value_ptr(lit->getDiffuse()));
				ImGui::Spacing();

				config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Specular);
				ImGui::ColorEdit3(UniformID::getStringStatic(ShaderParam::LightDirection::Specular).data()
					, glm::value_ptr(lit->getSpecular()));
				ImGui::Spacing();
			}
		});

	this->drawComponent<Camera>([](Component* com)
		{
			if (ImGui::CollapsingHeader("相机(Camera)"))
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
		});

	this->drawComponent<Skybox>([](Component* com)
		{

		});
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
	if (!mGameObject.lock())
	{
		return;
	}

	ImGui::Text("名称(Name):");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), mGameObject->getName().c_str());

	auto& components = mGameObject->getCompoents();
	for (auto com : components)
	{
		mDrawFunctions[com->getComponentTypeID()](com);
	}
}

void MyOverviewWindow::onUpdate()
{

}
