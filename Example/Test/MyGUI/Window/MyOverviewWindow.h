#pragma once

#include "../MyGUIContext.h"

class MyOverviewWindow : public GUIWindow
{
public:
	MyOverviewWindow();
	virtual ~MyOverviewWindow();

	void onSelectObject(GameObject* object);
private:

protected:
	void onRender() override;
	void onUpdate() override;


private:
	void drawDefault(Component* com) {}

private:
	template<typename Com>
	void drawComponent(std::function<void(Component*)>&& function)
	{
		auto id = Com::getComponentTypeIDStatic();
		if (mDrawFunctions.size() <= id)
		{
			mDrawFunctions.resize(id + 1, std::bind(&MyOverviewWindow::drawDefault, this, std::placeholders::_1));
		}

		mDrawFunctions[id] = std::move(function);
	}

private:
	bool mOpen;
	TinyWeakRef<GameObject> mGameObject;
	std::vector<std::function<void(Component*)>> mDrawFunctions;
};


