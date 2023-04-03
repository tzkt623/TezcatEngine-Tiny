#include "../OpenGL/GLHead.h"
#include "Core/Engine.h"

struct GLFWwindow;

using namespace tezcat::Tiny::Core;
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

		bool preInit(ResourceLoader* loader) override;
		bool onInit(ResourceLoader* loader) override;
		bool postInit(ResourceLoader* loader) override;

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
}
