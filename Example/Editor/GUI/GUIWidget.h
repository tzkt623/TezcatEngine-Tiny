#pragma once
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

#include "GUI.h"

namespace tezcat::Editor
{
	class GUI;

	class TINY_API GUIFunction
	{
	public:
		static float getFrameRate();
		static float getFrameRateTime();
	};

	class TINY_API GUIWidget
	{
	public:
		GUIWidget(const std::u8string& name);
		GUIWidget(const std::string& name);
		GUIWidget(const char* name);

		virtual ~GUIWidget();

		virtual void init() { }
		virtual void update();
		const char* getName() { return mName.c_str(); }

		void setName(const std::u8string& val)
		{
			mName = StringTool::fromU8(val);
		}

		void setName(const std::string& val) { mName = val; }

		virtual void open(GUI* gui) {}
		virtual void close() {}

	protected:
		virtual void onRender() = 0;
		virtual void onNotRender() {}

	protected:
		std::string mName;
	};

	class TINY_API GUIWindow : public GUIWidget
	{
	public:
		GUIWindow(const std::u8string& name);
		GUIWindow(const std::string& name);
		GUIWindow(const char* name);
		virtual ~GUIWindow();

	public:
		virtual bool begin();
		virtual void end();

		void update() override;
		void onRender() override;
		bool isClosed() const { return !mIsOpen; }
		void setClose() { mIsOpen = false; }
		GUI* getHost() { return mHost; }

		void setFocus();
	public:
		void open(GUI* gui) override;
		void close() override;

	protected:
		GUI* mHost;
		bool mIsOpen;
		bool mNeedFocus;
	};
}
