#include "GUIWidget.h"

namespace tezcat::Editor
{
	float GUIFunction::getFrameRate()
	{
		return ImGui::GetIO().Framerate;
	}

	float GUIFunction::getFrameRateTime()
	{
		return 1000.0f / ImGui::GetIO().Framerate;
	}

	//----------------------------------------------------------------------
	GUIWidget::GUIWidget(const std::u8string& name)
		: mName(StringTool::fromU8(name))
	{

	}

	GUIWidget::GUIWidget(const std::string& name)
		: mName(name)
	{

	}

	GUIWidget::GUIWidget(const char* name)
		: mName(name)
	{

	}

	GUIWidget::~GUIWidget()
	{

	}

	void GUIWidget::update()
	{
		this->onRender();
	}


	//-------------------------------------------------------
	GUIWindow::GUIWindow(const std::u8string& name)
		: GUIWidget(name)
		, mIsOpen(true)
	{

	}

	GUIWindow::GUIWindow(const std::string& name)
		: GUIWidget(name)
		, mIsOpen(true)
	{

	}

	GUIWindow::GUIWindow(const char* name)
		: GUIWidget(name)
		, mIsOpen(true)
	{

	}

	GUIWindow::~GUIWindow()
	{

	}

	bool GUIWindow::begin()
	{
		return ImGui::Begin(this->getName(), &mIsOpen);
	}

	void GUIWindow::end()
	{
		ImGui::End();
	}

	void GUIWindow::open(GUI* gui)
	{
		mHost = gui;
		mHost->addWindow(this);
	}

	void GUIWindow::close()
	{
		mIsOpen = false;
	}

	void GUIWindow::setFocus()
	{
		mNeedFocus = true;
	}

	void GUIWindow::onRender()
	{
		if (mNeedFocus)
		{
			mNeedFocus = false;
			ImGui::SetWindowFocus();
		}
	}

	void GUIWindow::update()
	{
		if (this->begin())
		{
			this->onRender();
		}
		this->end();
	}
}
