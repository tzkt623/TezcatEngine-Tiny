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
	GUIText* m_LabelDrawCall;
	GUIDragFloat2* m_MousePosition;
	GUIDragFloat2* m_MouseOffset;
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

class MyMainCameraWindow : public GUIWindow
{
public:
	MyMainCameraWindow();
	~MyMainCameraWindow();

	void init() override;

protected:
	void onUpdate() override;

private:
	GUIDragFloat3* m_Position;
	GUIDragFloat3* m_Rotation;
	GUIDragFloat3* m_Up;
	GUIDragFloat3* m_Right;
	GUIDragFloat3* m_Front;
	Camera* m_MainCamera;
};

