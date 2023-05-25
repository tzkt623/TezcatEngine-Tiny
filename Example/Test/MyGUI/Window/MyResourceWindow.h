#pragma once

#include "../MyGUIContext.h"

class MyResourceWindow : public GUIWindow
{
	CreateInstanceH(MyResourceWindow);

public:
	bool begin() override;
	void end() override;

protected:
	void onRender() override;
	void drawTree(std::filesystem::directory_entry& entry);

private:
	std::string mRootPath;
	std::filesystem::directory_entry mEntry;
	std::filesystem::path mPath;
};


