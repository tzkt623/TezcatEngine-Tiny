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
			ImGui::Text("投影(Projection)");
			static const char* projection_name = "error";
			IRenderObserver::ViewType view_type = camera->getViewType();
			switch (view_type)
			{
			case IRenderObserver::ViewType::Screen:
				break;
			case IRenderObserver::ViewType::Ortho:
			{
				projection_name = "正交(Ortho)";
				break;
			}
			case IRenderObserver::ViewType::Perspective:
			{
				projection_name = "透视(Perspective)";
				break;
			}
			default:
				break;
			}

			if (ImGui::BeginCombo("##Projection", projection_name))
			{
				ImGui::Selectable("正交(Ortho)");
				if (ImGui::IsItemClicked())
				{
					view_type = IRenderObserver::ViewType::Ortho;
				}

				ImGui::Selectable("透视(Perspective)");
				if (ImGui::IsItemClicked())
				{
					view_type = IRenderObserver::ViewType::Perspective;
				}

				ImGui::EndCombo();
			}

			switch (view_type)
			{
			case IRenderObserver::ViewType::Screen:
				break;
			case IRenderObserver::ViewType::Ortho:
			{
				float near_far[2] = { camera->getNear(),camera->getFar() };
				ImGui::Text("NearFar");
				ImGui::SameLine();
				ImGui::InputFloat2("##NearFar", near_far);

				camera->setOrtho(near_far[0], near_far[1]);
				break;
			}
			case IRenderObserver::ViewType::Perspective:
			{
				float fov = camera->getFOV();
				ImGui::Text("Fov    ");
				ImGui::SameLine();
				ImGui::DragFloat("##Fov", &fov, 1, 1, 180);

				float near_far[2] = { camera->getNear(),camera->getFar() };
				ImGui::Text("NearFar");
				ImGui::SameLine();
				ImGui::InputFloat2("##NearFar", near_far);

				camera->setPerspective(fov, near_far[0], near_far[1]);

				break;
			}
			default:
				break;
			}

			/*
			auto vinfo = camera->getViewportInfo();
			int oxoy[2] = { vinfo.OX, vinfo.OY };
			ImGui::Text("原点(Origin)");
			ImGui::SameLine();
			ImGui::InputInt2("##oxoy", oxoy);

			int wh[2] = { vinfo.Width, vinfo.Height };
			ImGui::Text("长宽(Width&Height)");
			ImGui::SameLine();
			ImGui::InputInt2("##wh", wh);
			camera->setViewRect(oxoy[0], oxoy[1], wh[0], wh[1]);
			*/

			int order = camera->getOrder();
			ImGui::Text("Order  ");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(120);
			ImGui::InputInt("##order", &order);
			camera->setOrder(order);

			ImGui::Separator();
			ImGui::Text("ClearOptions");
			auto clear_options = camera->getClearOption();
			static bool clear_color = (clear_options & ClearOption::CO_Color) == ClearOption::CO_Color;
			static bool clear_depth = (clear_options & ClearOption::CO_Depth) == ClearOption::CO_Depth;
			static bool clear_stencil = (clear_options & ClearOption::CO_Stencil) == ClearOption::CO_Stencil;
			static bool clear_skybox = (clear_options & ClearOption::CO_Skybox) == ClearOption::CO_Skybox;

			ImGui::Checkbox("Color  ", &clear_color);
			ImGui::SameLine(0, 20);
			ImGui::Checkbox("Depth  ", &clear_depth);
			ImGui::Checkbox("Stencil", &clear_stencil);
			ImGui::SameLine(0, 20);
			ImGui::Checkbox("Skybox ", &clear_skybox);

			ClearOptionID co_id = 0;
			if (clear_color)
			{
				co_id |= ClearOption::CO_Color;
			}

			if (clear_depth)
			{
				co_id |= ClearOption::CO_Depth;
			}

			if (clear_stencil)
			{
				co_id |= ClearOption::CO_Stencil;
			}

			if (clear_skybox)
			{
				co_id |= ClearOption::CO_Skybox;
			}
			camera->setClearOption(co_id);


			/*
			ImGui::Separator();
			ImGui::Text("朝向");
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
			*/
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
