#include "Core/Head/GLHead.h"
#include "Core/Engine.h"

struct GLFWwindow;

namespace tezcat::Tiny::Core
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

		bool preInit(ResourceLoader* loader) override;
		bool onInit(ResourceLoader* loader) override;
		bool postInit(ResourceLoader* loader) override;

		BaseGraphics* createGraphics() override;

	private:
		static void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
		static void mouseCursorPosCallBack(GLFWwindow* window, double xpos, double ypos);
		static void mouseScrollPosCallBack(GLFWwindow* window, double xoffset, double yoffset);

	public:
		GLFWwindow* getGLFWWindow() const { return m_Window; }

	private:
		GLFWwindow* m_Window;

		double m_OldTime;
		double m_NowTime;
	};
}