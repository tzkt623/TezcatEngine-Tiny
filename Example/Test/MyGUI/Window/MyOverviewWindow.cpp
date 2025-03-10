﻿#include "MyOverviewWindow.h"
#include "../../MyEvent.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(MyOverviewWindow)

	MyOverviewWindow::MyOverviewWindow()
		: GUIWindow("场景总览(Overview)")
		, mSelectedGameObject(nullptr)
	{

	}

	MyOverviewWindow::~MyOverviewWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	void MyOverviewWindow::init()
	{

	}

	void MyOverviewWindow::buildTree(std::list<TinyWeakRef<Transform>>& children)
	{
		for (auto& transform : children)
		{
			auto game_object = transform->getGameObject();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
			if (mSelectedGameObject == game_object)
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

			auto open = ImGui::TreeNodeEx(game_object->getName().c_str(), flags);
			if (mDragedTransform == nullptr && ImGui::BeginDragDropSource())
			{
				int s;
				if (ImGui::SetDragDropPayload("ObjectMove", &s, sizeof(int), ImGuiCond_Once))
				{
					mDragedTransform = transform.get();
					TINY_LOG_INFO("!!!!");
				}
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (auto payload = ImGui::AcceptDragDropPayload("ObjectMove"))
				{
					mDragedTransform->setParent(transform.get());
					mDragedTransform = nullptr;
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsItemClicked())
				{
					mSelectedGameObject = game_object;
					MyEvent::get()->dispatch({ MyEventID::Window_ObjectSelected, game_object });
				}
				else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("ObjectMenu");
				}
			}

			if (open)
			{
				if (has_children)
				{
					buildTree(*children);
				}

				ImGui::TreePop();
			}
		}
	}

	void MyOverviewWindow::onRender()
	{
		GUIWindow::onRender();

		if (SceneManager::empty())
		{
			mSelectedGameObject = nullptr;
			return;
		}

		ImGui::BeginChild("##list");

		auto& list = SceneManager::getCurrentScene()->getTransformList();
		this->buildTree(list);

		if (ImGui::BeginPopup("ObjectMenu"))
		{
			ImGui::Text("Aquarium");
			ImGui::EndPopup();
		}

		ImGui::EndChild();

		if (ImGui::BeginDragDropTarget())
		{
			if (auto payload = ImGui::AcceptDragDropPayload("ObjectMove"))
			{
				mDragedTransform->setParent(nullptr);
				mDragedTransform = nullptr;
			}
			ImGui::EndDragDropTarget();
		}


		/*
		for (auto& transform : list)
		{
			auto game_object = transform->getGameObject();

			ImGuiTreeNodeFlags flags = base_flags;
			if (mSelectedGameObject == game_object)
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

			auto open = ImGui::TreeNodeEx(game_object->getName().c_str(), flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				mSelectedGameObject = game_object;
				MyEvent::get()->dispatch(EventData
					{
						MyEventID::Window_ObjectSelected,
						game_object
					});
			}

			if (open)
			{
				if (has_children)
				{
					buildTree(*children);
				}
				ImGui::TreePop();
			}
		}
		*/
	}

	void MyOverviewWindow::end()
	{
		GUIWindow::end();

	}



}
