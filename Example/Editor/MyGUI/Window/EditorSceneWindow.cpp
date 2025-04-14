#include "EditorSceneWindow.h"
#include "../../EditorCamera.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(EditorSceneWindow);
	EditorSceneWindow::EditorSceneWindow()
		: GUIWindow("场景(Scene)")
		, mFixMouse(true)
		, mLastX(0)
		, mLastY(0)
		, mYaw(0)
		, mPitch(0)
		, mRoll(0)
		, mCameraMoveSpeed(100.0f)
		, mOldTime(0)
		, mCurrentTime(0)
	{

	}

	EditorSceneWindow::~EditorSceneWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	bool EditorSceneWindow::begin()
	{
		return ImGui::Begin(this->getName(), 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	}

	void EditorSceneWindow::onRender()
	{
		GUIWindow::onRender();

		if (ImGui::BeginMenuBar())
		{
			static bool bInfo;
			static PolygonMode polygon_mode = PolygonMode::Face;
			static std::string polygon_mode_name = "Face";
			ImGui::SetNextItemWidth(200);
			if (ImGui::BeginCombo("##PylgonMode", polygon_mode_name.c_str()))
			{
				if (ImGui::Selectable("Face##face0", polygon_mode == PolygonMode::Face))
				{
					polygon_mode = PolygonMode::Face;
					polygon_mode_name = "Face";
					Graphics::getInstance()->setPolygonMode(polygon_mode);
				}

				if (ImGui::Selectable("Line##line1", polygon_mode == PolygonMode::Line))
				{
					polygon_mode = PolygonMode::Line;
					polygon_mode_name = "Line";
					Graphics::getInstance()->setPolygonMode(polygon_mode);
				}

				if (ImGui::Selectable("Point##point2", polygon_mode == PolygonMode::Point))
				{
					polygon_mode = PolygonMode::Point;
					polygon_mode_name = "Point";
					Graphics::getInstance()->setPolygonMode(polygon_mode);
				}

				ImGui::EndCombo();
			}

			ImGui::Text("CameraSpeed");
			ImGui::SetNextItemWidth(120);
			ImGui::DragFloat("##editorcamera", &mCameraMoveSpeed);

			auto size = ImGui::CalcTextSize("Info");
			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - size.x);

			ImGui::MenuItem("Info", 0, &bInfo);
			if (bInfo)
			{
				auto pos = ImGui::GetCursorScreenPos();
				pos.y += ImGui::GetItemRectSize().y;

				this->drawInfo(pos);
			}

			ImGui::EndMenuBar();
		}

		if (ImGui::BeginChild("##Scene"))
		{
			if (!SceneManager::isEmpty() && CameraManager::isDataValied())
			{
				mViewPortPos = ImGui::GetItemRectMin();

				if (EditorContext::EditorTex2DColor)
				{
					ImVec2 display_size, offset, uv0, uv1, texture_size;
					if (EditorContext::EditorCamera)
					{
						texture_size = ImVec2((float)EditorContext::EditorTex2DColor->getWidth(), (float)EditorContext::EditorTex2DColor->getHeight());

						ImGuiHelper::fitImageToRect(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);
						EditorContext::EditorCamera->setViewRect(0, 0, display_size.x, display_size.y);
						EditorContext::sViewPortSize = display_size;

						ImGui::SetCursorPos(offset);
						ImGui::Image((ImTextureID)EditorContext::EditorTex2DColor->getTextureID()
									, display_size
									, uv0
									, uv1);
					}
					else
					{
						texture_size = ImVec2((float)EditorContext::EditorTex2DColor->getWidth(), (float)EditorContext::EditorTex2DColor->getHeight());
						ImGuiHelper::fitImageToRect(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);

						ImGui::SetCursorPos(offset);
						ImGui::Image((ImTextureID)EditorContext::EditorTex2DColor->getTextureID()
									, display_size
									, ImVec2(0, 1)
									, ImVec2(1, 0));
					}

					mFramePos.x = mViewPortPos.x + offset.x;
					mFramePos.y = mViewPortPos.y + offset.y;

					auto mouse_pos = ImGui::GetMousePos();
					mMousePos.x = (mouse_pos.x - mFramePos.x);
					mMousePos.y = ImGui::GetItemRectSize().y - (mouse_pos.y - mFramePos.y);

					if (EditorContext::EditorCamera)
					{
						if (EditorContext::SelectedGameObject)
						{
							ImGuizmo::SetOrthographic(false);
							ImGuizmo::SetDrawlist();
							auto window_pos = ImGui::GetItemRectMin();
							auto size = ImGui::GetItemRectSize();
							ImGuizmo::SetRect(window_pos.x, window_pos.y, size.x, size.y);

							auto transform = EditorContext::SelectedGameObject->getTransform();

							//这个matrix是一个世界坐标系的矩阵
							auto& transform_matrix = transform->getModelMatrix();

							ImGuizmo::Manipulate(glm::value_ptr(EditorContext::EditorCamera->getViewMatrix())
								, glm::value_ptr(EditorContext::EditorCamera->getProjectionMatrix())
								, EditorContext::SelectedObjectOperation
								, EditorContext::SelectedObjectMode
								, glm::value_ptr(transform_matrix));


							if (ImGuizmo::IsUsing())
							{
								glm::vec3 translation, scale, rotation;
								GLMHelper::decompose(transform_matrix, translation, rotation, scale);

								switch (EditorContext::SelectedObjectOperation)
								{
								case ImGuizmo::TRANSLATE:
									transform->setWorldPosition(translation);
									break;
								case ImGuizmo::ROTATE:
									transform->setWorldRotation(rotation);
									break;
								case ImGuizmo::SCALE:
									transform->setScale(scale);
									break;
								default:
									break;
								}
							}
						}
					}

					if (ImGui::IsItemHovered())
					{
						if (!ImGuizmo::IsOver() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
						{
							if (GameObjectManager::allowPickObject())
							{
								int32_t pos[2]
								{
									mMousePos.x,
									mMousePos.y
								};
								EngineEvent::getInstance()->dispatch({ EngineEventID::EE_ReadObjectID, pos });
							}
						}

						//InputSys::getInstance()->update();
					}
				}

				//-------------------------------------
				if (EditorContext::EditorCamera)
				{
					if (ImGui::IsItemHovered())
					{
						this->control(EditorContext::EditorCamera->getTransform());
					}
					EditorContext::EditorCamera->getTransform()->update();
					EditorContext::EditorCamera->preRender();

					//先剔除
					for (auto& index : EditorContext::EditorCamera->getCullLayerList())
					{
						//剔除到对应的渲染通道
						RenderObjectCache::culling(index, EditorContext::EditorCamera);
					}

					EditorContext::EditorCamera->getPipelineQueue()->addToPipeline();
				}
			}

			ImGui::EndChild();
		}
	}

	void EditorSceneWindow::drawInfo(ImVec2 pos)
	{
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_AlwaysAutoResize
			| ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoNav
			;

		if (ImGui::Begin("状态(State)", 0, window_flags))
		{
			//gpu
			ImGui::Text("GPU: %s", Profiler::GPU);

			//logic time
			ImGui::Text("LogicTime: %.1f ms", Profiler::LogicTime);

			//render time
			mFrameTimeBuffer.push_back((float)Profiler::RenderTime);
			if (mFrameTimeBuffer.size() > 40)
			{
				mFrameTimeBuffer.erase(mFrameTimeBuffer.begin());
			}
			ImGui::SetNextItemWidth(200);
			ImGui::PlotLines("Frame Times", &mFrameTimeBuffer[0], (int)mFrameTimeBuffer.size());
			ImGui::SameLine();
			ImGui::Text("%.1f ms", Profiler::RenderTime);

			//fps
			ImGui::Text("FPS: %.1f(%.3f ms/Frame)", GUIFunction::getFrameRate(), GUIFunction::getFrameRateTime());

			//draw
			ImGui::Text("PassCount: %d", Profiler::PassCount);
			ImGui::Text("DrawCount: %d", Profiler::DrawCall);
			ImGui::Separator();

			ImGui::Text("Memory");
			ImGui::Text("Used: %.3f mb", Profiler::getMemoryUse() / 1024.0f / 1024.0f);
			ImGui::Text("TotalID: %d", TinyGC::totalID());
			ImGui::SameLine();
			ImGui::Text("FreeID: %d", TinyGC::freeID());
			ImGui::SameLine();
			ImGui::Text("UsedID: %d", TinyGC::usedID());
			ImGui::Separator();

			ImGui::Text("ObjectCount: %d", GameObjectManager::getObjectCount());

			ImGui::Separator();

			//auto mouse_pos = ImGui::GetMousePos();
			//ImVec2 mouse_local(mouse_pos.x - mFramePos.x, mouse_pos.y - mFramePos.y);
			//mouse
			ImGui::InputFloat2("MousePosition", &mMousePos[0], "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat2("MouseOffset", glm::value_ptr(Profiler::mouseOffset), "%.3f", ImGuiInputTextFlags_ReadOnly);

			pos.x -= ImGui::GetWindowSize().x;
			ImGui::SetWindowPos(pos);

			ImGui::End();
		}
	}

	void EditorSceneWindow::control(Transform* transform)
	{

		bool flag = false;
		float3 position(0.0f);

		static auto yaw_pitch_function = [&transform, this](float yaw, float pitch, bool constrainPitch)
			{
				mYaw += yaw;
				mPitch += pitch;

				if (mYaw > 360.0f)
				{
					mYaw -= 360.0f;
				}
				else if (mYaw < -360.0f)
				{
					mYaw += 360.0f;
				}

				if (constrainPitch)
				{
					if (mPitch > 89.0f)
					{
						mPitch = 89.0f;
					}

					if (mPitch < -89.0f)
					{
						mPitch = -89.0f;
					}
				}

				transform->setRotation(mPitch, mYaw, mRoll);
			};

		//if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		//{
		//	if (ImGui::IsKeyPressed(ImGuiKey_LeftAlt))
		//	{
		//		flag = true;
		//		mControlMousePos = ImGui::GetMousePos();
		//		auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
		//		mOldControlMousePos = mControlMousePos;
		//
		//		position += transform->getRight() * delta.x * speed;
		//		position += transform->getUp() * delta.y * speed;
		//	}
		//}

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
		{
			static double old_time = ImGui::GetTime();
			double current_time = ImGui::GetTime();
			float delta = current_time - old_time;
			old_time = current_time;

			float speed = mCameraMoveSpeed * delta;

			if (ImGui::IsKeyPressed(ImGuiKey_W))
			{
				flag = true;
				position += transform->getForward() * speed;
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_S))
			{
				flag = true;
				position -= transform->getForward() * speed;
			}

			if (ImGui::IsKeyPressed(ImGuiKey_A))
			{
				flag = true;
				position -= transform->getRight() * speed;
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_D))
			{
				flag = true;
				position += transform->getRight() * speed;
			}

			if (ImGui::IsKeyPressed(ImGuiKey_R))
			{
				flag = true;
				position += transform->getUp() * speed;
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_F))
			{
				flag = true;
				position -= transform->getUp() * speed;
			}

			auto mouse_pos = ImGui::GetMousePos();
			if (mFixMouse)
			{
				mFixMouse = false;
				mLastX = mouse_pos.x;
				mLastY = mouse_pos.y;
			}
			else
			{
				float rollspeed = 20 * Engine::getDeltaTime();

				float offset_x = mouse_pos.x - mLastX;
				float offset_y = mLastY - mouse_pos.y;

				mLastX = mouse_pos.x;
				mLastY = mouse_pos.y;

				yaw_pitch_function(-offset_x * rollspeed, offset_y * rollspeed, true);
			}
		}
		else
		{
			mFixMouse = true;
		}

		if (flag)
		{
			transform->translate(position);
		}
	}
}
