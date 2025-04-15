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

	class EditorCamera;
	class EditorContext
	{
	public:
		static void init();
		static ValueConfig* getValueConfig(const UniformID& ID);
		static void clearOnPopScene();

		static void matrix4(glm::mat4& mat4);
		static void transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

	public:
		static const ImVec2 UV0;
		static const ImVec2 UV1;
		static ImVec2 sViewPortSize;
		static GameObject* SelectedGameObject;
		static std::vector<GameObject*> DeleteArray;

		static ImGuizmo::OPERATION SelectedObjectOperation;
		static ImGuizmo::MODE SelectedObjectMode;


		static EditorCamera* EditorCamera;
		static bool EditorSceneFoucsed;
		static FrameBuffer* EditorFrameBuffer;
		static Texture2D* EditorTex2DColor;

		static bool IsPlaying;
		static bool IsPause;
		static bool IsFocusOnGameView;

	public:
		static file_path openFile(const char* filter);

		static void createCamera();
		static void createBuildinModel(const std::string& model);
		static void createDirectionLight();

		static void beginFrame();
		static void endFrame();

	private:
		static std::vector<ValueConfig*> mValueConfigAry;
		
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
