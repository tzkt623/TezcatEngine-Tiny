#pragma once

#include "../GUI/GUIWidget.h"

namespace tezcat::Editor
{
	struct ValueConfig
	{
		bool isColor;
		float speed;
		float min;
		float max;

		// 	ValueConfig()
		// 		: isColor(false)
		// 		, speed(0)
		// 		, min(0)
		// 		, max(0)
		// 	{}


	};



	class MyGUIContext
	{
	public:
		static MyGUIContext& getInstance()
		{
			static MyGUIContext instance;
			return instance;
		}
		ValueConfig* getValueConfig(const UniformID& ID);
		static bool isDragResource()
		{
			return sIsDragResource;
		}

		static void beginDragResource()
		{
			sIsDragResource = true;
		}

		static void endDragResource()
		{
			sIsDragResource = false;
		}

	private:
		MyGUIContext();
		~MyGUIContext();
		void initValueConfig();

	private:

		std::vector<ValueConfig*> mValueConfigAry;
		static bool sIsDragResource;

	public:
		static void matrix4(glm::mat4& mat4);
		static void transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

	public:
		static const ImVec2 UV0;
		static const ImVec2 UV1;
	};

	class MyWindow : public GUIWindow
	{
	public:
		MyWindow(const std::string& name)
			: GUIWindow(name)
		{
		}

		virtual ~MyWindow()
		{
			mOnCloseCallback();
		}

		void setCloseCallback(const std::function<void()>& callback)
		{
			mOnCloseCallback = callback;
		}

	protected:
		std::function<void()> mOnCloseCallback;
	};

#define TINY_EDITOR_WINDOW_INSTANCE_H(class_name)\
	class_name();\
public:\
	virtual ~class_name();\
	static class_name* create(GUI* host);\
private:\
	static class_name* sInstance;

#define TINY_EDITOR_WINDOW_INSTANCE_CPP(class_name)\
	class_name* class_name::create(GUI* host)\
	{\
		if (sInstance == nullptr)\
		{\
			sInstance = new class_name();\
			sInstance->open(host);\
		}\
		return sInstance;\
	}\
	class_name* class_name::sInstance = nullptr;

#define TINY_EDITOR_WINDOW_DELETE_INSTACNE() TINY_ASSERT(sInstance == this); sInstance = nullptr;
}
