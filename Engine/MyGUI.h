#pragma once

#include "Tiny.h"

class MyInfoWindow : public GUIWindow
{
public:
	MyInfoWindow();
	~MyInfoWindow();

	void init() override;

protected:
	void onUpdate() override;

private:
	GUIText* m_LabelFPS;
	GUIText* m_LabelPass;
};

class MyObjectWindow : public GUIWindow
{
public:
	MyObjectWindow();
	~MyObjectWindow();

	void init() override;

protected:
	void onUpdate() override;

private:
	GUIDragFloat3* m_Position;
};
