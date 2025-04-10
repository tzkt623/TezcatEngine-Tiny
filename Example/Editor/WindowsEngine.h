#include "include/Tiny.h"

struct GLFWwindow;

namespace tezcat::Editor
{
	class EditorMainWindow;
	class GUI;
	/// <summary>
	/// engine负责所有window相关的操作
	/// graphics只负责draw
	///
	///	editor初始化
	/// editor开始渲染
	/// 并不启动engine的内容
	/// 
	/// </summary>
	class TINY_API WindowsEditor : public Engine
	{
	protected:
		bool preInit() override;
		bool onInit() override;
		bool postInit() override;
		void beforeLoop() override;
		void endLoop() override;
		void preUpdate() override;
		void postUpdate() override;
		void onUpdate() override;

		void notifyRender() override;

		BaseGraphics* createGraphics() override;

	private:
		GLFWwindow* mWindow;
		GUI* mGUI;
		EditorMainWindow* mEditorWindow;

		double mTimeOld;
		double mTimeNow;
	};
}
