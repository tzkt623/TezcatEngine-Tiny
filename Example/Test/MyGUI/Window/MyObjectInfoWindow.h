#pragma once

#include "../MyGUIContext.h"

class MyObjectInfoWindow : public GUIWindow
{
	CreateInstanceH(MyObjectInfoWindow);
public:
	void onSelectObject(GameObject* object);
private:

protected:
	void onRender() override;
	int resetName(ImGuiInputTextCallbackData* data);


private:
	void drawDefault(Component* com) {}

private:
	template<typename Com>
	void drawComponent(std::function<void(Component*)>&& function)
	{
		auto id = Com::getComponentTypeIDStatic();
		if (mDrawFunctions.size() <= id)
		{
			mDrawFunctions.resize(id + 1, std::bind(&MyObjectInfoWindow::drawDefault, this, std::placeholders::_1));
		}

		mDrawFunctions[id] = std::move(function);
	}

private:
	std::string mNameBuffer;
	TinyWeakRef<GameObject> mGameObject;
	std::vector<std::function<void(Component*)>> mDrawFunctions;
};


