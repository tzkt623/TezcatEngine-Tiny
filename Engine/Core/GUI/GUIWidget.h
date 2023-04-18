#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/ImGuiHead.h"
#include "../Head/GLMHead.h"

#include "Utility/Utility.h"


namespace tezcat::Tiny::Core
{
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
			mName = Utility::StringTool::fromU8(val);
		}

		void setName(const std::string& val) { mName = val; }

		virtual void open() {}
		virtual void close() {}

	protected:
		virtual void onRender() = 0;
		virtual void onUpdate() = 0;

	private:
		std::string mName;
	};

	class TINY_API GUIParentWidget : public GUIWidget
	{
		using GUIWidget::GUIWidget;

	public:
		void update() final;

		void addChild(GUIWidget* child)
		{
			mChildren.emplace_back(child);
		}

		void removeChild(GUIWidget* child)
		{
			auto it = mChildren.begin();
			while (it != mChildren.end())
			{
				if ((*it) == child)
				{
					mChildren.erase(it);
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
		std::vector<GUIWidget*> mChildren;
	};


	class TINY_API GUIWindow : public GUIParentWidget
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


	class TINY_API GUIDragFloat : public GUIWidget
	{
	public:
		GUIDragFloat(const std::u8string& name);
		virtual ~GUIDragFloat();

		std::function<void(float* data)> postFunction;
		void setMinMax(float min, float max)
		{
			m_Min = min;
			m_Max = max;
		}

		void setSpeed(float speed)
		{
			m_Speed = speed;
		}


	protected:
		void onUpdate() override {}


	protected:
		float m_Speed;
		float m_Min;
		float m_Max;

	private:
		static void defaultPostFunction(float* data) {}
	};


	class TINY_API GUIDragFloat1 : public GUIDragFloat
	{
	public:
		GUIDragFloat1(const std::u8string& name);
		virtual ~GUIDragFloat1();

		void setFloat(float x)
		{
			mData = x;
		}

		float getFloat()
		{
			return mData;
		}

	protected:
		void onRender() override;

	private:
		float mData;
	};

	class TINY_API GUIDragFloat2 : public GUIDragFloat
	{
	public:
		GUIDragFloat2(const std::u8string& name);
		virtual ~GUIDragFloat2();

		void setFloat2(float x, float y)
		{
			mData[0] = x;
			mData[1] = y;
		}

		void setFloat2(float* v)
		{
			mData[0] = v[0];
			mData[1] = v[1];
		}

		float* getFloat2()
		{
			return mData;
		}

	protected:
		void onRender() override;

	private:
		float mData[2];
	};

	class TINY_API GUIDragFloat3 : public GUIDragFloat
	{
	public:
		GUIDragFloat3(const std::u8string& name);
		virtual ~GUIDragFloat3();

		void setFloat3(float x, float y, float z)
		{
			mData[0] = x;
			mData[1] = y;
			mData[2] = z;
		}

		void setFloat3(float* v)
		{
			mData[0] = v[0];
			mData[1] = v[1];
			mData[2] = v[2];
		}

		float* getFloat3()
		{
			return mData;
		}

	protected:
		void onRender() override;

	private:
		float mData[3];
	};

	class TINY_API GUIMatrix4 : public GUIWidget
	{
	public:
		GUIMatrix4(const std::u8string& name);
		virtual ~GUIMatrix4();

		void setMat(glm::mat4* mat)
		{
			mData = mat;
		}

	protected:
		void onRender() override;
		void onUpdate() override {}

	private:
		glm::mat4* mData;
	};

	class TINY_API GUIText : public GUIWidget
	{
	public:
		GUIText(const std::u8string& name);
		virtual ~GUIText();

		void setData(const char* data)
		{
			mData = data;
		}

		void setData(std::string&& data)
		{
			mData = std::move(data);
		}

		void onRender() override;
		void onUpdate() override {}

	private:
		std::string mData;
	};

	class TINY_API GUIImage : public GUIWidget
	{
	public:
		GUIImage(const std::u8string& name);
		virtual ~GUIImage();

		void refresh();

	protected:
		void onRender() override;
		void onUpdate() override;

	private:
	};
}
