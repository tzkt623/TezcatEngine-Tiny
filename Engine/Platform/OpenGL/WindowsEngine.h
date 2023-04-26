#include "../OpenGL/GLHead.h"
#include "Core/Engine.h"

struct GLFWwindow;

using namespace tezcat::Tiny;

namespace tezcat::Tiny
{
	class GUI;
}

namespace tezcat::Tiny::GL
{
	class TINY_API WindowsEngine : public Engine
	{
	public:
		WindowsEngine();
		~WindowsEngine();

	protected:
		void beforeLoop() override;
		void preUpdate() override;
		void postUpdate() override;
		void endLoop() override;
		void onUpdate() override;

		bool preInit() override;
		bool onInit() override;
		bool postInit() override;

		BaseGraphics* createGraphics() override;

	private:
		static void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
		static void mouseCursorPosCallBack(GLFWwindow* window, double xpos, double ypos);
		static void mouseScrollPosCallBack(GLFWwindow* window, double xoffset, double yoffset);

	public:
		GLFWwindow* getGLFWWindow() const { return mWindow; }

	private:
		GLFWwindow* mWindow;

		double mTimeOld;
		double mTimeNow;
	};

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
	public:
		void setGLVersion(int major, int minor);
		GUI* getGUI();

	protected:
		bool preInit() override;
		bool onInit() override;
		bool postInit() override;
		void beforeLoop() override;
		void endLoop() override;
		void preUpdate() override;
		void postUpdate() override;
		void onUpdate() override;
		BaseGraphics* createGraphics() override;


	private:
		GLFWwindow* mWindow;

		double mTimeOld;
		double mTimeNow;

		int mGLMajor;
		int mGLMinor;

		GUI* mGUI;
	};
}
