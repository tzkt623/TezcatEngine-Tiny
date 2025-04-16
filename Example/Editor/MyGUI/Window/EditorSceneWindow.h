#pragma once

#include "../EditorContext.h"

namespace tezcat::Editor
{
	class EditorCamera;
	class EditorSceneWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(EditorSceneWindow);

	protected:
		bool begin() override;
		void onRender() override;

		void drawInfo(ImVec2 pos);

	private:
		void control(Transform* tansform);

	private:
		std::vector<float> mFrameTimeBuffer;
		ImVec2 mViewPortPos;
		ImVec2 mMousePos;
		ImVec2 mFramePos;
		float mLastX;
		float mLastY;
		float mYaw;
		float mPitch;
		float mRoll;
		float mCameraMoveSpeed;
		bool mFixMouse;

		ImVec2 mControlMousePos;
		ImVec2 mOldControlMousePos;
		float3 mSnapTranslation;
		float3 mSnapRotation;
		float3 mSnapScale;
		bool mUseSnap;

		double mOldTime;
		double mCurrentTime;
	};
}
