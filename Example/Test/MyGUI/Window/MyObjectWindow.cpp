#include "MyObjectWindow.h"


MyObjectWindow::MyObjectWindow()
	: GUIWindow("场景总览(Scene)")
	, mSelectedItem(-1)
{
	MyGUIContext::getInstance().setObjectWindow(this);
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
	for (auto tran : list)
	{
		index++;
		ImGui::Selectable(tran->getName().c_str(), mSelectedItem == index);
		if (ImGui::IsItemClicked())
		{
			mSelectedItem = index;
			MyGUIContext::getInstance().selectObject(tran);
		}
	}
}
