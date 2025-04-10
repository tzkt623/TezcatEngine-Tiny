#pragma once
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
