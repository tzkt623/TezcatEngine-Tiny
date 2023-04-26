#include "MyObjectWindow.h"
#include "../../MyEvent.h"


MyObjectWindow::MyObjectWindow()
	: GUIWindow("场景总览(Scene)")
	, mSelectedItem(-1)
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
		return;
	}

	auto& list = SceneMgr::getInstance()->getCurrentScene()->getObjectList();
	int index = -1;
	for (auto game_object : list)
	{
		index++;
		ImGui::Selectable(game_object->getName().c_str(), mSelectedItem == index);
		if (ImGui::IsItemClicked())
		{
			mSelectedItem = index;
			MyEvent::get()->dispatch(EventData
				{
					MyEventID::Window_ObjectSelected,
					game_object
				});
		}
	}
}
