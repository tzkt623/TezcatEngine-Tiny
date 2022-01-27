#pragma once
#include <type_traits>

namespace tezcat::Tiny::Module
{
	class InputSystem;
	class Inputer
	{
	public:
		Inputer() = default;
		virtual ~Inputer() = default;
		virtual void processInput(InputSystem* system) = 0;
		virtual void onEnter() = 0;
		virtual void onExit() = 0;
		virtual void onPause() = 0;

		virtual void mouseButtonCallBack(int button, int action, int mods) = 0;
		virtual void mouseCursorPosCallBack(double xpos, double ypos) = 0;
		virtual void mouseScrollPosCallBack(double xoffset, double yoffset) = 0;
	};

//	template<typename I, typename = typename std::enable_if<std::is_base_of<Inputer, I>::value>::type>
	template<typename I>
	class InputerT : public Inputer
	{
	public:
		static I* getInstace()
		{
			static I instance;
			return &instance;
		}
	};
}

