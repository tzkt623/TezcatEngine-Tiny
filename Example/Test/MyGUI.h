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
	GUIText* m_LabelGPU;
	GUIText* m_LabelMemory;
	GUIText* m_LabelLogicTime;
	GUIText* m_LabelRenderTime;
	GUIText* m_LabelFPS;
	GUIText* m_LabelPass;
	GUIText* m_LabelDrawCall;
	GUIDragFloat2* m_MousePosition;
	GUIDragFloat2* m_MouseOffset;
	GUIImage* mImageDepth;
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
	GUIDragFloat3* mPosition;
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
	GUIText* m_Info;

	GUIDragFloat3* mPosition;
	GUIDragFloat3* m_Rotation;
	GUIDragFloat3* m_Up;
	GUIDragFloat3* m_Right;
	GUIDragFloat3* m_Front;
	Camera* mMainCamera;
};

