#pragma once

#include "../EditorContext.h"
namespace tezcat::Editor
{
	class EditorOverviewWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(EditorOverviewWindow)
	public:
		void init() override;
		void buildTree(std::list<TinyWeakRef<Transform>>& children);
		void onRender() override;
		void end() override;

	private:
		void scrollToCurrentObject();
		void createPickPath(Transform* transform);

	private:
		//GameObject* mSelectedGameObject;
		Transform* mDragedTransform;
		bool mPickObject;
		std::stack<Transform*> mPickPath;
	};
}
