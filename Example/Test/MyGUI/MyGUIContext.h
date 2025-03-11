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

	class MyDragDropController
	{
	public:
		std::string_view dragData(const std::filesystem::path& path);
		std::tuple<bool, std::string> dropData();
		std::string getFilePath() { return mFilePath.string(); }

	private:
		bool mIsDragResource;
		FileType mFileType;
		std::filesystem::path mFilePath;
		std::string mDragName;
	};

	class MyTextureSizeHelper
	{
	public:
		static void calculate(const ImVec2& inTextureSize, const ImVec2& inWindowSize
			, ImVec2& outDisplaySize, ImVec2& outOffsetToCenter, ImVec2& outUV0, ImVec2& outUV1);

		static void fitImageToRect(const ImVec2& inWindowSize, const ImVec2& inImageSize, ImVec2& outDisplaySize, ImVec2& outOffsetToCenter);
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
		static MyDragDropController DragDropController;
		static ImVec2 sViewPortSize;
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
