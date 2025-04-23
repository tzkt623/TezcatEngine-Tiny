#include "EditorObjectInfoWindow.h"
#include "../../EditorEvent.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(EditorObjectInfoWindow);
	EditorObjectInfoWindow::EditorObjectInfoWindow()
		: GUIWindow("物体信息(ObjectInfo)")
		, mDrawFunctions()
	{
		mShaderValueFunctionArray.resize((int32_t)UniformType::Count, {});
		mShaderValueFunctionArray[(int32_t)UniformType::Bool] = [](Uniform* uniform, UniformValueConfig* config, const int32_t& id)
			{
				auto f1 = (UniformI1*)uniform;
				ImGui::Text(config->editorName.c_str());
				ImGui::PushID(id);
				ImGui::SetNextItemWidth(-FLT_MIN);
				bool v = f1->value;
				if (ImGui::Checkbox("", &v))
				{
					f1->value = v;
				}
				ImGui::PopID();

				ImGui::Spacing();
			};

		mShaderValueFunctionArray[(int32_t)UniformType::Int] = [](Uniform* uniform, UniformValueConfig* config, const int32_t& id)
			{
				auto f1 = (UniformI1*)uniform;
				ImGui::Text(config->editorName.c_str());
				ImGui::PushID(id);
				ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::DragInt("", &f1->value, 0.02f);
				ImGui::PopID();

				ImGui::Spacing();
			};

		mShaderValueFunctionArray[(int32_t)UniformType::Float] = [](Uniform* uniform, UniformValueConfig* config, const int32_t& id)
			{
				auto f1 = (UniformF1*)uniform;
				auto range = (RangeFloat*)config->range.get();
				ImGui::Text(config->editorName.c_str());
				ImGui::PushID(id);
				ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::DragFloat("", &f1->value, 0.02f, range->min, range->max);
				ImGui::PopID();

				ImGui::Spacing();
			};
		mShaderValueFunctionArray[(int32_t)UniformType::Float3] = [](Uniform* uniform, UniformValueConfig* config, const int32_t& id)
			{
				auto f3 = (UniformF3*)uniform;

				ImGui::Text(config->editorName.c_str());
				ImGui::PushID(id);
				ImGui::SetNextItemWidth(-FLT_MIN);
				if (config->constraint == ShaderMemberConstraint::Color)
				{
					ImGui::ColorEdit3("", glm::value_ptr(f3->value));
				}
				else
				{
					auto range = (RangeFloat*)config->range.get();
					ImGui::DragFloat3("", glm::value_ptr(f3->value), 0.02f, range->min, range->max);
				}

				ImGui::PopID();

				ImGui::Spacing();
			};
		mShaderValueFunctionArray[(int32_t)UniformType::Float4] = [](Uniform* uniform, UniformValueConfig* config, const int32_t& id)
			{
				auto f4 = (UniformF4*)uniform;

				ImGui::Text(config->editorName.c_str());
				ImGui::PushID(id);
				ImGui::SetNextItemWidth(-FLT_MIN);
				if (config->constraint == ShaderMemberConstraint::Color)
				{
					ImGui::ColorEdit4("", glm::value_ptr(f4->value));
				}
				else
				{
					auto range = (RangeFloat*)config->range.get();
					ImGui::DragFloat4("", glm::value_ptr(f4->value), 0.02f, range->min, range->max);
				}

				ImGui::PopID();

				ImGui::Spacing();
			};
		mShaderValueFunctionArray[(int32_t)UniformType::Tex2D] = [](Uniform* uniform, UniformValueConfig* config, const int32_t& id)
			{
				auto tex = (UniformTex2D*)uniform;
				ImGui::Text(config->editorName.c_str());
				if (tex->value)
				{
					ImGui::Image((ImTextureID)tex->value->getTextureID()
						, ImVec2(100, 100)
						, ImVec2(0, 1)
						, ImVec2(1, 0));
				}
				else
				{
					ImGui::PushID(id);
					ImGui::ImageButton(""
						, 0
						, ImVec2(100, 100)
						, ImVec2(0, 1)
						, ImVec2(1, 0));
					ImGui::PopID();
				}

				if (ImGui::IsItemHovered())
				{
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
					{
						tex->set(nullptr);
					}
				}

				ImGuiHelper::dropResource([&tex](file_path path)
				{
					Texture2D* img = ResourceManager::loadAndSave<Texture2D>(path);
					if (img)
					{
						tex->set(img);
					}
				});

				ImGui::Spacing();
			};

		this->drawComponent<Transform>([](Component* com)
		{
			if (ImGui::CollapsingHeader("坐标(Transform)", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto transform = static_cast<Transform*>(com);
				if (ImGui::BeginTable("##tabel", 2, ImGuiTableFlags_SizingStretchProp))
				{
					bool is_dirty = false;

					ImGui::TableSetupColumn("##0", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("##1", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Position");
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-FLT_MIN);
					is_dirty |= ImGui::DragFloat3("##Position", glm::value_ptr(transform->getPosition()), 0.02f);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Rotation");
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-FLT_MIN);
					is_dirty |= ImGui::DragFloat3("##Rotation", glm::value_ptr(transform->getRotation()), 0.02f);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Scale");
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-FLT_MIN);
					is_dirty |= ImGui::DragFloat3("##Scale", glm::value_ptr(transform->getScale()), 0.02f);

					ImGui::EndTable();

					if (is_dirty)
					{
						transform->markDirty();
					}
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


			}
		});

		this->drawComponent<MeshRenderer>([this](Component* com)
		{
			mMeshRenderer = static_cast<MeshRenderer*>(com);
			if (ImGui::CollapsingHeader("网格渲染器(MeshRenderer)", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto mr = static_cast<MeshRenderer*>(com);
				ImGui::Text("Mesh");
				auto vertex = mr->getVertex();
				ImGui::Text("Name: %s", vertex->getEngineName().toData());
				ImGui::Text("VertexID: %d", vertex->getVertexID());
				ImGui::Text("IndexCount: %d", vertex->getIndexCount());
				ImGui::Text("Face: %d", vertex->getIndexCount() / 3);
			}
		});

		this->drawComponent<DirectionalLight>([](Component* com)
		{
			if (ImGui::CollapsingHeader("区域光(DirectionalLight)", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_SizingStretchProp))
				{
					auto lit = static_cast<DirectionalLight*>(com);
					ImGui::TableSetupColumn("##0", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("##1", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					//auto config = EditorContext::getValueConfig(ShaderParam::LightDirection::Ambient);
					//ImGui::Text(UniformID::staticGetString(ShaderParam::LightDirection::Ambient).data());
					ImGui::Text("Ambient");
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::ColorEdit3("##0", glm::value_ptr(lit->getAmbient()));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					//config = EditorContext::getValueConfig(ShaderParam::LightDirection::Diffuse);
					//ImGui::Text(UniformID::staticGetString(ShaderParam::LightDirection::Diffuse).data());
					ImGui::Text("Diffuse");
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::ColorEdit3("##1", glm::value_ptr(lit->getDiffuse()));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					//config = EditorContext::getValueConfig(ShaderParam::LightDirection::Specular);
					//ImGui::Text(UniformID::staticGetString(ShaderParam::LightDirection::Specular).data());
					ImGui::Text("Specular");
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::ColorEdit3("##2", glm::value_ptr(lit->getSpecular()));

					ImGui::EndTable();
				}
			}
		});

		this->drawComponent<Camera>([](Component* com)
		{
			if (ImGui::CollapsingHeader("相机(Camera)", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::BeginTable("242##table", 2, ImGuiTableFlags_SizingStretchProp))
				{
					auto camera = static_cast<Camera*>(com);
					auto transform = camera->getTransform();

					ImGui::TableSetupColumn("##0", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("##1", ImGuiTableColumnFlags_WidthStretch);
					//ImGui::TableHeadersRow();

					{//Base
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("MainCamera");
						ImGui::TableNextColumn();
						auto is_main = camera->isMain();
						ImGui::SetNextItemWidth(-FLT_MIN);
						if (ImGui::Checkbox("##MainCmaera01", &is_main))
						{
							if (is_main)
							{
								camera->setMain();
							}
						}

						int sorting = camera->getSortingID();
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Sorting");
						ImGui::TableNextColumn();
						ImGui::SetNextItemWidth(-FLT_MIN);

						if (ImGui::InputInt("##sorting", &sorting))
						{
							camera->setSortingID(sorting);
						}
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Dummy(ImVec2(-1, ImGui::GetTextLineHeight()));

					{
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

						bool flag_change_projection = false;
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Projection");
						ImGui::TableNextColumn();
						ImGui::SetNextItemWidth(-FLT_MIN);

						if (ImGui::BeginCombo("##Projection", projection_name))
						{
							ImGui::Selectable("正交(Ortho)", view_type == ViewType::Ortho);
							if (ImGui::IsItemClicked())
							{
								view_type = ViewType::Ortho;
								flag_change_projection = true;
							}

							ImGui::Selectable("透视(Perspective)", view_type == ViewType::Perspective);
							if (ImGui::IsItemClicked())
							{
								view_type = ViewType::Perspective;
								flag_change_projection = true;
							}

							ImGui::EndCombo();
						}

						switch (view_type)
						{
						case ViewType::Screen:
							break;
						case ViewType::Ortho:
						{
							bool flag = false;
							float near_far[2] = { camera->getNear(), camera->getFar() };

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("NearFar");
							ImGui::TableNextColumn();
							ImGui::SetNextItemWidth(-FLT_MIN);

							flag = ImGui::InputFloat2("##NearFar", near_far);

							if (flag || flag_change_projection)
							{
								camera->setOrtho(near_far[0], near_far[1]);
							}
							break;
						}
						case ViewType::Perspective:
						{
							bool flag = false;
							int fov = camera->getFOV();
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Fov");
							ImGui::TableNextColumn();
							ImGui::SetNextItemWidth(-FLT_MIN);

							flag |= ImGui::SliderInt("##Fov", &fov, 1, 180);

							float near_far[2] = { camera->getNear(),camera->getFar() };
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Clipping Planes");
							ImGui::TableNextColumn();
							ImGui::SetNextItemWidth(-FLT_MIN);

							flag |= ImGui::InputFloat2("##NearFar", near_far);

							if (flag || flag_change_projection)
							{
								camera->setPerspective(fov, near_far[0], near_far[1]);
							}

							break;
						}
						default:
							break;
						}
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Dummy(ImVec2(-1, ImGui::GetTextLineHeight()));

					{//ViewRect
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						bool flag = false;
						auto& vinfo = camera->getViewRect();
						int oxoy[2] = { vinfo.OX, vinfo.OY };
						ImGui::Text("Origin");
						ImGui::TableNextColumn();
						ImGui::SetNextItemWidth(-FLT_MIN);

						flag |= ImGui::InputInt2("##oxoy", oxoy);

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						int wh[2] = { vinfo.Width, vinfo.Height };
						ImGui::Text("Width&Height");
						ImGui::TableNextColumn();
						ImGui::SetNextItemWidth(-FLT_MIN);

						flag |= ImGui::InputInt2("##wh", wh);

						if (flag)
						{
							camera->setViewRect(oxoy[0], oxoy[1], wh[0], wh[1]);
						}
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Dummy(ImVec2(-1, ImGui::GetTextLineHeight()));

					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("ClearOptions");
						ImGui::TableNextColumn();
						ImGui::SetNextItemWidth(-FLT_MIN);

						auto clear_options = (uint32_t)camera->getClearOption();
						bool flag_clear = false;
						if (ImGui::BeginCombo("##ClearOptions", "ClearOptions"))
						{
							flag_clear |= ImGui::CheckboxFlags("Color", &clear_options, ClearOption::CO_Color);
							flag_clear |= ImGui::CheckboxFlags("Depth", &clear_options, ClearOption::CO_Depth);
							flag_clear |= ImGui::CheckboxFlags("Stencil", &clear_options, ClearOption::CO_Stencil);
							flag_clear |= ImGui::CheckboxFlags("Skybox ", &clear_options, ClearOption::CO_Skybox);

							ImGui::EndCombo();
						}

						if (flag_clear)
						{
							camera->setClearOption(clear_options);
						}

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("ClearColor");
						ImGui::TableNextColumn();
						ImGui::SetNextItemWidth(-FLT_MIN);
						float4 clear_color = camera->getClearColor();
						if (ImGui::ColorEdit4("##ClearColor01", &clear_color[0]))
						{
							camera->setClearColor(clear_color);
						}

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("RenderTarget");
						ImGui::TableNextColumn();
						ImGui::SetNextItemWidth(-FLT_MIN);
						//ImGui::PushID("#RenderTarget");
						ImGui::Text(camera->getFrameBuffer()->getName().c_str());
						//ImGui::TextWrapped(camera->getFrameBuffer()->getName().c_str());
						//ImGui::ItemAdd(ImGui::GetCurrentContext()->LastItemData.Rect, ImGui::GetCurrentContext()->LastItemData.ID);
						//ImGuiHelper::textWithBG("{}", camera->getFrameBuffer()->getName());
						if (ImGui::IsItemHovered())
						{
							ImGuiHelper::itemHighLight();
						}
						//ImGui::PopID();
					}

					ImGui::EndTable();
				}
			}
		});

		this->drawComponent<ShadowCaster>([](Component* com)
		{
			if (ImGui::CollapsingHeader("阴影投射器(ShadowCaster)", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto caster = static_cast<ShadowCaster*>(com);

				ImGui::Image((ImTextureID)caster->getShadowTexutre()->getTextureID()
					, ImVec2(256, 256)
					, ImVec2(1, 0)
					, ImVec2(0, 1));
			}
		});
	}

	EditorObjectInfoWindow::~EditorObjectInfoWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	void EditorObjectInfoWindow::onRender()
	{
		GUIWindow::onRender();

		if (EditorContext::SelectedGameObject == nullptr)
		{
			return;
		}

		ImGui::Text("名称(Name):");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), EditorContext::SelectedGameObject->getName().c_str());

		mMeshRenderer = nullptr;

		auto& components = EditorContext::SelectedGameObject->getCompoents();
		for (auto com : components)
		{
			if (com)
			{
				mDrawFunctions[com->getComponentTypeID()](com);
			}
		}

		if (mMeshRenderer)
		{
			this->drawExtraMaterial();

			if (ImGui::BeginPopup("ShaderFiles##001"))
			{
				auto mt = mMeshRenderer->getMaterial();
				auto shader = mt->getShader();
				auto& shaders = ShaderManager::getAllowUseShaders();

				for (auto& pair : shaders)
				{
					if (ImGui::Selectable(pair.first.data(), pair.second == shader))
					{
						mt->setShader(pair.second);
					}
				}

				ImGui::EndPopup();
			}
		}

		ImGui::Separator();
		ImGui::Dummy(ImVec2(-1, ImGui::GetTextLineHeight()));
		if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeight() * 2.0f)))
		{

		}
	}

	int EditorObjectInfoWindow::resetName(ImGuiInputTextCallbackData* data)
	{
		if (data->EventChar)
		{
			EditorContext::SelectedGameObject->setName(data->Buf);
			return 0;
		}

		return 1;
	}

	void EditorObjectInfoWindow::drawExtraMaterial()
	{
		if (ImGui::CollapsingHeader("材质(Material)", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& mr = mMeshRenderer;
			auto mt = mr->getMaterial();
			auto shader = mt->getShader();
			auto& uniforms = mt->getUniforms();

			if (ImGui::Button("Shader##ds"))
			{
				//MyEvent::get()->dispatch({ MyEventID::Window_OpenShaderManager, mMeshRenderer });

				ImGui::OpenPopup("ShaderFiles##001");
			}
			ImGui::SameLine();
			ImGui::Text(shader->getName().c_str());


			uint32_t widget_id = 0;
			for (uint32_t i = 0; i < uniforms.size(); i++)
			{
				auto info = shader->getUserUniformValueConfig(i);
				auto uniform = uniforms[i];

				mShaderValueFunctionArray[(int32_t)uniform->getType()](uniform, info, widget_id++);
			}
		}
	}

}
