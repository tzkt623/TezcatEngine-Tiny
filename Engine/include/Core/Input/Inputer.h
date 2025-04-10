#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Component/FlyController.h"

namespace tezcat::Tiny
{
	class InputSystem;

	class TINY_API Inputer
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
		virtual void setController(FlyController* val) = 0;
	};

//	template<typename I, typename = typename std::enable_if<std::is_base_of<Inputer, I>::value>::type>
	template<typename I>
	class TINY_API InputerT : public Inputer
	{
	public:
		static I* getInstance()
		{
			static I instance;
			return &instance;
		}

		void setController(FlyController* val)
		{
			mController = val;
		}

	protected:
		TinyWeakRef<FlyController> mController;
	};

	class TINY_API EmptyInputer : public InputerT<EmptyInputer>
	{
	public:
		void processInput(InputSystem* system) override;
		void onEnter() override;
		void onExit() override;
		void onPause() override;
		void mouseButtonCallBack(int button, int action, int mods) override;
		void mouseCursorPosCallBack(double xpos, double ypos) override;
		void mouseScrollPosCallBack(double xoffset, double yoffset) override;
	};
}

