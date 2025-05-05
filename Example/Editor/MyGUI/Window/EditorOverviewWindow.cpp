#include "EditorOverviewWindow.h"
#include "../../EditorEvent.h"
#include "../../EditorCamera.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(EditorOverviewWindow);

	EditorOverviewWindow::EditorOverviewWindow()
		: GUIWindow("场景总览(Overview)")
		//, mSelectedGameObject(nullptr)
		, mPickObject(false)
	{

	}

	EditorOverviewWindow::~EditorOverviewWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	void EditorOverviewWindow::init()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_SelectObject, this,
			[this](const EventData& data)
			{
				EditorContext::SelectedGameObject = (GameObject*)data.userData;
				if (EditorContext::SelectedGameObject)
				{
					this->createPickPath(EditorContext::SelectedGameObject->getTransform());
				}
			});
	}

	void EditorOverviewWindow::buildTree(std::list<TinyWeakRef<Transform>>& children)
	{
		int32_t child_id = 0;
		for (auto& transform : children)
		{
			auto game_object = transform->getGameObject();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
			if (EditorContext::SelectedGameObject == game_object)
			{
				flags |= ImGuiTreeNodeFlags_Selected;
			}

			auto children = transform->getChildren();
			auto has_children = children && !children->empty();
			if (has_children)
			{
				flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			}
			else
			{
				flags |= ImGuiTreeNodeFlags_Leaf;
			}


			if (!mPickPath.empty())
			{
				//如果当前对象在选取路径上
				if (transform.get() == mPickPath.top())
				{
					//展开节点给用户看
					mPickPath.pop();
					ImGui::SetNextItemOpen(true);
				}
			}

			ImGui::PushID(child_id++);
			auto open = ImGui::TreeNodeEx(game_object->getName().c_str(), flags);

			//使当前单位成为抓取目标
			if (mDragedTransform == nullptr && ImGui::BeginDragDropSource())
			{
				if (ImGui::SetDragDropPayload("ObjectMove", nullptr, 0, ImGuiCond_Once))
				{
					mDragedTransform = transform.get();
					TINY_LOG_INFO("!!!!");
				}
				ImGui::EndDragDropSource();
			}

			//使当前单位成为投放目标
			if (mDragedTransform != nullptr && ImGui::BeginDragDropTarget())
			{
				//投放一个对象到当前对象的子节点中
				if (ImGui::AcceptDragDropPayload("ObjectMove"))
				{
					//一定要先添加父类
					mDragedTransform->setParent(transform.get());
					this->createPickPath(mDragedTransform);
					EditorContext::SelectedGameObject = mDragedTransform->getGameObject();

					mDragedTransform = nullptr;
				}
				ImGui::EndDragDropTarget();
			}

			//点击控件选择对象
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					EditorContext::SelectedGameObject = game_object;
					EditorEvent::get()->dispatch({ EditorEventID::Window_ObjectSelected, game_object });
					EngineEvent::getInstance()->dispatch({ EngineEventID::EE_FocusObject, game_object });
					EditorContext::EditorCamera->lookAt(EditorContext::SelectedGameObject->getTransform()->getWorldPosition());
				}
				else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					EditorContext::SelectedGameObject = game_object;
					EditorEvent::get()->dispatch({ EditorEventID::Window_ObjectSelected, game_object });
				}
			}

			//从屏幕中选择物品时
			//将滚动条移动到当前物品的位置
			if (mPickObject)
			{
				if (EditorContext::SelectedGameObject == game_object)
				{
					mPickObject = false;
					this->scrollToCurrentObject();
				}
			}

			if (ImGui::BeginPopupContextItem())
			{
				ImGui::Text("ObjectControl");
				ImGui::Separator();

				if (ImGui::Button("DeleteObject"))
				{
					EditorContext::DeleteArray.push_back(game_object);
					if (game_object == EditorContext::SelectedGameObject)
					{
						EditorContext::SelectedGameObject = nullptr;
					}
					//game_object->deleteObject();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				if (has_children)
				{
					buildTree(*children);
				}
				ImGui::TreePop();
			}

			ImGui::PopID();
		}
	}

	void EditorOverviewWindow::onRender()
	{
		GUIWindow::onRender();

		if (SceneManager::isEmpty())
		{
			return;
		}

		ImGui::BeginChild("##list");

		auto& list = SceneManager::getCurrentScene()->getTransformList();
		this->buildTree(list);

		ImGui::EndChild();

		ImGuiHelper::dropResource([](file_path path)
		{
			EngineEvent::getInstance()->dispatch({ EngineEventID::EE_LoadModel, &path });
		});

		if (ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload("ObjectMove"))
			{
				EditorContext::SelectedGameObject = mDragedTransform->getGameObject();
				mDragedTransform->setParent(nullptr);
				mDragedTransform = nullptr;
				mPickObject = true;
			}
			ImGui::EndDragDropTarget();
		}
	}

	void EditorOverviewWindow::end()
	{
		GUIWindow::end();
	}

	void EditorOverviewWindow::scrollToCurrentObject()
	{
		//-----------------------------------------------
		// 核心：滚动到当前控件的位置
		//-----------------------------------------------
		// 1. 获取控件的屏幕坐标
		ImVec2 item_min = ImGui::GetItemRectMin(); // 控件的左上角
		ImVec2 item_max = ImGui::GetItemRectMax(); // 控件的右下角

		// 2. 获取窗口的可视区域信息
		ImVec2 window_pos = ImGui::GetWindowPos();   // 窗口的屏幕坐标
		float window_height = ImGui::GetWindowHeight(); // 窗口可视高度
		float current_scroll_y = ImGui::GetScrollY();   // 当前滚动位置

		// 3. 计算控件在窗口内的相对位置
		float item_top_rel = item_min.y - window_pos.y + current_scroll_y;
		float item_bottom_rel = item_max.y - window_pos.y + current_scroll_y;

		// 4. 判断是否需要调整滚动条
		if (item_top_rel < current_scroll_y)
		{
			// 控件在可视区域上方：滚动到控件顶部
			ImGui::SetScrollY(item_top_rel);
		}
		else if (item_bottom_rel > current_scroll_y + window_height)
		{
			// 控件在可视区域下方：滚动到控件底部
			ImGui::SetScrollY(item_bottom_rel - window_height);
		}
	}

	void EditorOverviewWindow::createPickPath(Transform* transform)
	{
		while (true)
		{
			auto temp = transform->getParent();
			if (temp == nullptr)
			{
				mPickObject = true;
				break;
			}
			else
			{
				mPickPath.push(temp);
				transform = temp;
			}
		}
	}

}
