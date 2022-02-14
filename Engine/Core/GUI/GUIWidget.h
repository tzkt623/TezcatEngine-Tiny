#pragma once
#include "../Head/CppHead.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	class GUIFunction
	{
	public:
		static float getFrameRate();
		static float getFrameRateTime();
	};

	class GUIWidget
	{
	public:
		GUIWidget(const std::u8string& name);
		GUIWidget(const std::string& name);
		GUIWidget(const char* name);

		virtual ~GUIWidget();

		virtual void init() { }

		virtual void update();

		const char* getName() { return m_Name.c_str(); }

		void setName(const std::u8string& val)
		{
			m_Name = Utility::Tools::fromU8(val);
		}

		void setName(const std::string& val) { m_Name = val; }

		virtual void open() {}
		virtual void close() {}

	protected:
		virtual void onRender() = 0;
		virtual void onUpdate() = 0;

	private:
		std::string m_Name;
	};

	class GUIParentWidget : public GUIWidget
	{
		using GUIWidget::GUIWidget;

	public:
		void update() final;

		void addChild(GUIWidget* child)
		{
			m_Children.emplace_back(child);
		}

		void removeChild(GUIWidget* child)
		{
			auto it = m_Children.begin();
			while (it != m_Children.end())
			{
				if ((*it) == child)
				{
					m_Children.erase(it);
					break;
				}
				it++;
			}
		}
	protected:
		virtual void beginParent() = 0;
		virtual void endParent() = 0;
		void onRender() override;

	private:
		std::vector<GUIWidget*> m_Children;
	};


	class GUIWindow : public GUIParentWidget
	{
	public:
		GUIWindow(const std::u8string& name);
		GUIWindow(const std::string& name);
		GUIWindow(const char* name);
		virtual ~GUIWindow();

	public:
		void beginParent() override;
		void endParent() override;

	public:
		void open() override;
		void close() override;
	};


	class GUIDragFloat : public GUIWidget
	{
	public:
		GUIDragFloat(const std::u8string& name);
		virtual ~GUIDragFloat();

		std::function<void(float* data)> postFunction;

	protected:
		void onUpdate() override {}

	protected:
		float m_Speed;
		float m_Min;
		float m_Max;

	private:
		static void defaultPostFunction(float* data) {}
	};

	class GUIDragFloat2 : public GUIDragFloat
	{
	public:
		GUIDragFloat2(const std::u8string& name);
		~GUIDragFloat2();

		void setFloat2(float x, float y)
		{
			m_Data[0] = x;
			m_Data[1] = y;
		}

		void setFloat2(float* v)
		{
			m_Data[0] = v[0];
			m_Data[1] = v[1];
		}

		float* getFloat2()
		{
			return m_Data;
		}

	protected:
		void onRender() override;

	private:
		float m_Data[2];
	};

	class GUIDragFloat3 : public GUIDragFloat
	{
	public:
		GUIDragFloat3(const std::u8string& name);
		~GUIDragFloat3();

		void setFloat3(float x, float y, float z)
		{
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
		}

		void setFloat3(float* v)
		{
			m_Data[0] = v[0];
			m_Data[1] = v[1];
			m_Data[2] = v[2];
		}

		float* getFloat3()
		{
			return m_Data;
		}

	protected:
		void onRender() override;

	private:
		float m_Data[3];
	};

	class GUIText : public GUIWidget
	{
	public:
		GUIText(const std::u8string& name);
		~GUIText();

		void setData(const char* data)
		{
			m_Data = data;
		}

		void setData(std::string&& data)
		{
			m_Data = std::move(data);
		}

		void onRender() override;
		void onUpdate() override {}

	private:
		std::string m_Data;
	};
}