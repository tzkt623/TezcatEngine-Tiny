#include "MyObjectWindow.h"


MyObjectWindow::MyObjectWindow()
	: GUIWindow(u8"物体信息(Object Info)")
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
