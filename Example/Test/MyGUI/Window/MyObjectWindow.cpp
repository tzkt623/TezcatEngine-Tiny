#include "MyObjectWindow.h"
#include "../../MyEvent.h"


MyObjectWindow::MyObjectWindow()
	: GUIWindow("场景总览(Scene)")
	, mSelectedGameObject(nullptr)
{

}

MyObjectWindow::~MyObjectWindow()
{

}

void MyObjectWindow::init()
{

}


void MyObjectWindow::onUpdate()
{
	if (SceneMgr::getInstance()->empty())
	{
		mSelectedGameObject = nullptr;
		return;
	}

	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_SpanAvailWidth;

	std::function<void(std::vector<TinyWeakRef<Transform>>&)> buildTree =
		[&buildTree, this](std::vector<TinyWeakRef<Transform>>& children)
	{
		for (auto& transform : children)
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
	};


	auto& list = SceneMgr::getInstance()->getCurrentScene()->getTransformList();
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


}
