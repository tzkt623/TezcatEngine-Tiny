#include "MyObjectInfoWindow.h"
#include "../../MyEvent.h"

CreateInstanceCPP(MyObjectInfoWindow);
MyObjectInfoWindow::MyObjectInfoWindow()
	: GUIWindow("物体信息(ObjectInfo)")
	, mGameObject(nullptr)
	, mDrawFunctions()
	, mNameBuffer(256, '\0')
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
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::DragFloat3("##Position", glm::value_ptr(transform->getPosition()), 0.03f);

			ImGui::Text("Rotation");
			ImGui::SameLine();
			ImGui::DragFloat3("##Rotation", glm::value_ptr(transform->getRotation()), 0.03f);

			ImGui::Text("Scale   ");
			ImGui::SameLine();
			ImGui::DragFloat3("##Scale", glm::value_ptr(transform->getScale()), 0.03f);
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

					ImGui::Text(UniformID::getStringStatic(f1->ID).data());
					ImGui::DragFloat("##0"
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
						ImGui::Text(UniformID::getStringStatic(f3->ID).data());
						ImGui::ColorEdit3("##1"
										 , glm::value_ptr(f3->value));
					}
					else
					{
						ImGui::Text(UniformID::getStringStatic(f3->ID).data());
						ImGui::DragFloat3("##2"
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
					ImGui::Image((ImTextureID)tex->value->getTextureID()
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
			ImGui::Text(UniformID::getStringStatic(ShaderParam::LightDirection::Ambient).data());
			ImGui::ColorEdit3("##0", glm::value_ptr(lit->getAmbient()));
			ImGui::Spacing();

			config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Diffuse);
			ImGui::Text(UniformID::getStringStatic(ShaderParam::LightDirection::Diffuse).data());
			ImGui::ColorEdit3("##1", glm::value_ptr(lit->getDiffuse()));
			ImGui::Spacing();

			config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Specular);
			ImGui::Text(UniformID::getStringStatic(ShaderParam::LightDirection::Specular).data());
			ImGui::ColorEdit3("##2", glm::value_ptr(lit->getSpecular()));
		}
	});

	this->drawComponent<Camera>([](Component* com)
	{
		if (ImGui::CollapsingHeader("相机(Camera)"))
		{
			auto camera = static_cast<Camera*>(com);
			auto transform = camera->getTransform();

			ImGui::Text("操作<CTRL> [W A S D] [R:Up] [F:Down]");
			ImGui::Text("位置(Position)");
			ImGui::SameLine();
			ImGui::InputFloat3("##位置", glm::value_ptr(transform->getPosition()));

			ImGui::Text("旋转(Rotation)");
			ImGui::SameLine();
			ImGui::InputFloat3("##旋转(Rotation)", glm::value_ptr(transform->getRotation()));

			ImGui::Text("缩放(Scale)   ");
			ImGui::SameLine();
			ImGui::InputFloat3("##缩放(Scale)", glm::value_ptr(transform->getScale()));
			ImGui::Spacing();

			auto up = transform->getUp();
			auto forward = transform->getForward();
			auto right = transform->getRight();

			ImGui::Text("上(Up)");
			ImGui::SameLine();
			ImGui::InputFloat3("##上(Up)", glm::value_ptr(up));

			ImGui::Text("前(Forward)");
			ImGui::SameLine();
			ImGui::InputFloat3("##前(Forward)", glm::value_ptr(forward));

			ImGui::Text("右(Right)");
			ImGui::SameLine();
			ImGui::InputFloat3("##右(Right)", glm::value_ptr(right));
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
		if (ImGui::CollapsingHeader("天空盒(Skybox)"))
		{

		}
	});

	this->drawComponent<ShadowCaster>([](Component* com)
	{
		if (ImGui::CollapsingHeader("阴影投射器(ShadowCaster)"))
		{
			auto caster = static_cast<ShadowCaster*>(com);

			ImGui::Image((ImTextureID)caster->getShadwowTexutre()->getTextureID()
				, ImVec2(256, 256)
				, ImVec2(1, 0)
				, ImVec2(0, 1));
		}
	});
}

MyObjectInfoWindow::~MyObjectInfoWindow()
{
	DeleteInstance();
}

void MyObjectInfoWindow::onSelectObject(GameObject* object)
{
	mGameObject = object;
}

void MyObjectInfoWindow::onRender()
{
	GUIWindow::onRender();

	if (!mGameObject.lock())
	{
		return;
	}

	//mNameBuffer.assign(mGameObject->getName().c_str());

	ImGui::Text("名称(Name):");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), mGameObject->getName().c_str());
	//ImGui::InputText("123", mNameBuffer.data(), mNameBuffer.capacity(), 0, &MyOverviewWindow::resetName);

	auto& components = mGameObject->getCompoents();
	for (auto com : components)
	{
		mDrawFunctions[com->getComponentTypeID()](com);
	}
}

int MyObjectInfoWindow::resetName(ImGuiInputTextCallbackData* data)
{
	if (data->EventChar)
	{
		mGameObject->setName(data->Buf);
		return 0;
	}

	return 1;
}
