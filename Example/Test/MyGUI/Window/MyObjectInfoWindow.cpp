#include "MyObjectInfoWindow.h"
#include "../../MyEvent.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(MyObjectInfoWindow)
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
				if (ImGui::BeginTable("##tabel", 2, ImGuiTableFlags_SizingStretchProp))
				{
					ImGui::TableSetupColumn("##0", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("##1", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow(ImGuiTableRowFlags_None);
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Position");
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(200);
					ImGui::DragFloat3("##Position", glm::value_ptr(transform->getPosition()), 0.03f);

					ImGui::TableNextRow(ImGuiTableRowFlags_None);
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Rotation");
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(200);
					ImGui::DragFloat3("##Rotation", glm::value_ptr(transform->getRotation()), 0.03f);

					ImGui::TableNextRow(ImGuiTableRowFlags_None);
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Scale");
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(200);
					ImGui::DragFloat3("##Scale", glm::value_ptr(transform->getScale()), 0.03f);

					ImGui::EndTable();
				}

				static bool open_matrix_info = false;
				ImGui::Checkbox("矩阵信息(MatrixInfo)", &open_matrix_info);
				if (open_matrix_info)
				{
					int widget_id = 0;
					auto& style = ImGui::GetStyle();
					float space = style.ItemInnerSpacing.x;

					auto draw_matrix = [&widget_id, &space](const char* label, float4x4& mat)
						{
							const float button_size = ImGui::GetFrameHeight();
							ImGui::PushID(widget_id++);
							ImGui::Text(label);

							ImGui::PushItemWidth(ImGui::CalcItemWidth() / 4.0f);
							//ImGui::PushItemWidth(-(ImGui::GetWindowContentRegionWidth() - ImGui::CalcItemWidth()));
							ImGui::LabelText("##X", "X");
							ImGui::SameLine(0.0f, space);
							ImGui::LabelText("##Y", "Y");
							ImGui::SameLine(0.0f, space);
							ImGui::LabelText("##Z", "Z");
							ImGui::SameLine(0.0f, space);
							ImGui::LabelText("##W", "W");
							ImGui::PopItemWidth();

							ImGui::InputFloat4("##001", &mat[0][0]);
							ImGui::InputFloat4("##002", &mat[1][0]);
							ImGui::InputFloat4("##003", &mat[2][0]);
							ImGui::InputFloat4("##004", &mat[3][0]);

							ImGui::PopID();
						};

					auto& mat4x4 = transform->getModelMatrix();
					draw_matrix("Local2World", mat4x4);
					auto inv_mat4x4 = glm::inverse(mat4x4);
					draw_matrix("World2Local", inv_mat4x4);

					// 				ImGui::Text("Local2World");
					// 				ImGui::InputFloat4("##001", &mat4x4[0][0], "%.7f");
					// 				ImGui::InputFloat4("##002", &mat4x4[1][0], "%.7f");
					// 				ImGui::InputFloat4("##003", &mat4x4[2][0], "%.7f");
					// 				ImGui::InputFloat4("##004", &mat4x4[3][0], "%.7f");
					// 
					// 				ImGui::Text("World2Local");
					// 				auto inv_mat4x4 = glm::inverse(mat4x4);
					// 				ImGui::InputFloat4("x", &inv_mat4x4[0][0], "%.7f");
					// 				ImGui::InputFloat4("y", &inv_mat4x4[1][0], "%.7f");
					// 				ImGui::InputFloat4("z", &inv_mat4x4[2][0], "%.7f");
					// 				ImGui::InputFloat4("w", &inv_mat4x4[3][0], "%.7f");

					auto& local_to_world_matrix = transform->getLocalToWorldMatrix();

					ImGui::Text("Quat");
					auto rotate = transform->getWorldRotation();
					ImGui::InputFloat3("##rotate", &rotate[0]);

					// 				auto qx = glm::angleAxis(glm::radians(rotate.x), Transform::XAxis);
					// 				auto qy = glm::angleAxis(glm::radians(rotate.y), Transform::YAxis);
					// 				auto qz = glm::angleAxis(glm::radians(rotate.z), Transform::ZAxis);
					// 				glm::quat quat = qy * qx * qz;

					glm::quat quat = glm::quat_cast(local_to_world_matrix);
					ImGui::InputFloat4("quat", &quat[0]);
					ImGui::InputFloat3("eulerAngles", &glm::degrees(glm::eulerAngles(quat))[0]);

					//30,60,10
					glm::quat test(0.8446119, -0.265384, -0.4615897, 0.05600987);
					ImGui::InputFloat3("##test", &glm::degrees(glm::eulerAngles(test))[0]);

				}



				// 			ImGui::Text("Position");
				// 			ImGui::SameLine();
				// 			ImGui::DragFloat3("##Position", glm::value_ptr(transform->getPosition()), 0.03f);
				// 
				// 			ImGui::Text("Rotation");
				// 			ImGui::SameLine();
				// 			ImGui::DragFloat3("##Rotation", glm::value_ptr(transform->getRotation()), 0.03f);
				// 
				// 			ImGui::Text("Scale   ");
				// 			ImGui::SameLine();
				// 			ImGui::DragFloat3("##Scale", glm::value_ptr(transform->getScale()), 0.03f);
				transform->markDirty();
			}
		});

		this->drawComponent<MeshRenderer>([](Component* com)
		{
			if (ImGui::CollapsingHeader("网格渲染器(MeshRenderer)"))
			{
				uint32_t widget_id = 0;
				auto mr = static_cast<MeshRenderer*>(com);
				ImGui::Text("Mesh");
				auto vertex = mr->getVertex();
				ImGui::Text("Name: %s", vertex->getEngineName().toData());
				ImGui::Text("VertexID: %d", vertex->getVertexID());
				ImGui::Text("VertexCount: %d", vertex->getVertexCount());
				ImGui::Text("IndexCount: %d", vertex->getIndexCount());
				ImGui::Text("Face: %d", vertex->getIndexCount() / 3);
				ImGui::Separator();

				ImGui::Text("Material");
				auto mt = mr->getMaterial();
				auto shader = mt->getShader();
				auto& uniforms = mt->getUniforms();
				for (uint32_t i = 0; i < uniforms.size(); i++)
				{
					auto info = shader->getUniformValueConfig(i);
					auto uniform = uniforms[i];

					switch (uniform->getType())
					{
					case UniformType::Float:
					{
						auto f1 = (UniformF1*)uniform;
						auto range = (RangeFloat*)info->range.get();
						ImGui::Text(info->editorName.c_str());
						ImGui::PushID(widget_id++);
						ImGui::DragFloat("", &f1->value, 0.02f, range->min, range->max);
						ImGui::PopID();

						ImGui::Spacing();
						break;
					}
					case UniformType::Float3:
					{
						auto f3 = (UniformF3*)uniform;

						ImGui::Text(info->editorName.c_str());
						ImGui::PushID(widget_id++);
						if (info->constraint == ShaderConstraint::Color)
						{
							ImGui::ColorEdit3("", glm::value_ptr(f3->value));
						}
						else
						{
							auto range = (RangeFloat*)info->range.get();
							ImGui::DragFloat3("", glm::value_ptr(f3->value), 0.02f, range->min, range->max);
						}

						ImGui::PopID();

						ImGui::Spacing();
						break;
					}
					case UniformType::Tex2D:
					{
						auto tex = (UniformTex2D*)uniform;
						ImGui::Text(info->editorName.c_str());
						ImGui::Image((ImTextureID)tex->value->getTextureID()
									, ImVec2(100, 100)
									, ImVec2(0, 1)
									, ImVec2(1, 0));

						if (ImGui::IsItemHovered())
						{
							ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						}

						if (ImGui::BeginDragDropTarget())
						{
							auto [flag, drop_name] = MyGUIContext::DragDropController.dropData();
							if (flag)
							{
								auto payload = ImGui::AcceptDragDropPayload(drop_name.data());
								if (payload)
								{
									Image* img = Image::create();
									if (img->openFile(MyGUIContext::DragDropController.getFilePath()))
									{
										Texture2D* new_tex = Texture2D::create();
										new_tex->setImage(img);
										new_tex->generate();

										tex->set(new_tex);
									}
								}
							}
							ImGui::EndDragDropTarget();
						}

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
				if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_SizingStretchProp))
				{
					auto lit = static_cast<DirectionalLight*>(com);
					ImGui::TableSetupColumn("##0", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("##1", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					auto config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Ambient);
					ImGui::Text(UniformID::getStringStatic(ShaderParam::LightDirection::Ambient).data());
					ImGui::TableNextColumn();
					ImGui::ColorEdit3("##0", glm::value_ptr(lit->getAmbient()));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Diffuse);
					ImGui::Text(UniformID::getStringStatic(ShaderParam::LightDirection::Diffuse).data());
					ImGui::TableNextColumn();
					ImGui::ColorEdit3("##1", glm::value_ptr(lit->getDiffuse()));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					config = MyGUIContext::getInstance().getValueConfig(ShaderParam::LightDirection::Specular);
					ImGui::Text(UniformID::getStringStatic(ShaderParam::LightDirection::Specular).data());
					ImGui::TableNextColumn();
					ImGui::ColorEdit3("##2", glm::value_ptr(lit->getSpecular()));

					ImGui::EndTable();
				}
			}
		});

		this->drawComponent<Camera>([](Component* com)
		{
			if (ImGui::CollapsingHeader("相机(Camera)"))
			{
				if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders))
				{
					auto camera = static_cast<Camera*>(com);
					auto transform = camera->getTransform();

					ImGui::TableSetupColumn("##0", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("##1", ImGuiTableColumnFlags_WidthStretch);

					static const char* projection_name = "error";
					ViewType view_type = camera->getViewType();
					switch (view_type)
					{
					case ViewType::Screen:
						break;
					case ViewType::Ortho:
					{
						projection_name = "正交(Ortho)";
						break;
					}
					case ViewType::Perspective:
					{
						projection_name = "透视(Perspective)";
						break;
					}
					default:
						break;
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Projection");
					ImGui::TableNextColumn();
					if (ImGui::BeginCombo("##Projection", projection_name))
					{
						ImGui::Selectable("正交(Ortho)");
						if (ImGui::IsItemClicked())
						{
							view_type = ViewType::Ortho;
						}

						ImGui::Selectable("透视(Perspective)");
						if (ImGui::IsItemClicked())
						{
							view_type = ViewType::Perspective;
						}

						ImGui::EndCombo();
					}

					switch (view_type)
					{
					case ViewType::Screen:
						break;
					case ViewType::Ortho:
					{
						float near_far[2] = { camera->getNear(),camera->getFar() };

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("NearFar");
						ImGui::TableNextColumn();
						ImGui::InputFloat2("##NearFar", near_far);

						camera->setOrtho(near_far[0], near_far[1]);
						break;
					}
					case ViewType::Perspective:
					{
						float fov = camera->getFOV();
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Fov");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##Fov", &fov, 1, 1, 180);

						float near_far[2] = { camera->getNear(),camera->getFar() };
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("NearFar");
						ImGui::TableNextColumn();
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

					int order = camera->getOrderID();
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Order");
					ImGui::TableNextColumn();
					ImGui::InputInt("##order", &order);
					camera->setOrderID(order);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("ClearOptions");
					ImGui::TableNextColumn();

					auto clear_options = (uint32_t)camera->getClearOption();
					bool clear_color = (clear_options & ClearOption::CO_Color) == ClearOption::CO_Color;
					bool clear_depth = (clear_options & ClearOption::CO_Depth) == ClearOption::CO_Depth;
					bool clear_stencil = (clear_options & ClearOption::CO_Stencil) == ClearOption::CO_Stencil;
					bool clear_skybox = (clear_options & ClearOption::CO_Skybox) == ClearOption::CO_Skybox;
					if (ImGui::BeginCombo("##ClearOptions", "ClearOptions"))
					{
						ImGui::CheckboxFlags("Color", &clear_options, ClearOption::CO_Color);
						ImGui::CheckboxFlags("Depth", &clear_options, ClearOption::CO_Depth);
						ImGui::CheckboxFlags("Stencil", &clear_options, ClearOption::CO_Stencil);
						ImGui::CheckboxFlags("Skybox ", &clear_options, ClearOption::CO_Skybox);

						ImGui::EndCombo();
					}

					camera->setClearOption(clear_options);

					// 				ImGui::Text("视图矩阵(ViewMatrix)");
					// 				MyGUIContext::matrix4(camera->getViewMatrix());
					// 				ImGui::Spacing();
					// 
					// 				ImGui::Text("模型矩阵(ModelMatrix)");
					// 				MyGUIContext::matrix4(camera->getTransform()->getModelMatrix());

					ImGui::EndTable();
				}

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
				*/
			}
		});

		this->drawComponent<ShadowCaster>([](Component* com)
		{
			if (ImGui::CollapsingHeader("阴影投射器(ShadowCaster)"))
			{
				auto caster = static_cast<ShadowCaster*>(com);

				ImGui::Image((ImTextureID)caster->getShadowTexutre()->getTextureID()
					, ImVec2(256, 256)
					, ImVec2(1, 0)
					, ImVec2(0, 1));
			}
		});
	}

	MyObjectInfoWindow::~MyObjectInfoWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
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
			if (com)
			{
				mDrawFunctions[com->getComponentTypeID()](com);
			}
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
}
