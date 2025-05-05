/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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

	}

	void GUIWindow::update()
	{
		if (this->begin())
		{
			if (mNeedFocus)
			{
				mNeedFocus = false;
				ImGui::SetWindowFocus();
			}

			this->onRender();
		}
		else
		{
			this->onNotRender();
		}
		this->end();
	}
}
