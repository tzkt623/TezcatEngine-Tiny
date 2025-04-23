#pragma once
/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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

		static bool IsShowNormal;
		static float NormalLength;

		static bool IsShowMeshFrame;
		static float MeshFrameLineWidth;

	public:
		static file_path openFile(const char* filter);

		static void createCamera();
		static void createBuildinModel(const std::string& model);
		static void createDirectionLight();

		static void beginFrame();
		static void endFrame();

		static void showNormal();
		static void hideNormal();

		static void showMeshFrame();
		static void hideMeshFrame();

	private:
		static std::vector<ValueConfig*> mValueConfigAry;
		static ReplacedPipelinePass* mShowNormalPass;
		static ReplacedPipelinePass* mShowMeshFramePass;
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
