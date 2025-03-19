#include "MyGUIContext.h"
#include "Window/MyObjectInfoWindow.h"

namespace tezcat::Editor
{
	const ImVec2 MyGUIContext::UV0(0, 1);
	const ImVec2 MyGUIContext::UV1(1, 0);
	MyDragDropController MyGUIContext::DragDropController;
	ImVec2 MyGUIContext::sViewPortSize;

	MyGUIContext::MyGUIContext()
		: mValueConfigAry(UniformID::allStringCount(), nullptr)
	{
		this->initValueConfig();
	}


	MyGUIContext::~MyGUIContext()
	{

	}


	ValueConfig* MyGUIContext::getValueConfig(const UniformID& ID)
	{
		return mValueConfigAry[ID.toUID()];
	}

	void MyGUIContext::initValueConfig()
	{
		mValueConfigAry[ShaderParam::LightDirection::Ambient.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
		mValueConfigAry[ShaderParam::LightDirection::Diffuse.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
		mValueConfigAry[ShaderParam::LightDirection::Specular.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
	}


	bool MyGUIContext::sIsDragResource = false;

	//-------------------------------------------------------------
	//
	//
	//
	//
	void MyGUIContext::matrix4(glm::mat4& mat4)
	{
		ImGui::Text("x");
		ImGui::SameLine();
		ImGui::InputFloat4("##x", glm::value_ptr(mat4[0]));

		ImGui::Text("y");
		ImGui::SameLine();
		ImGui::InputFloat4("##y", glm::value_ptr(mat4[1]));

		ImGui::Text("z");
		ImGui::SameLine();
		ImGui::InputFloat4("##z", glm::value_ptr(mat4[2]));

		ImGui::Text("w");
		ImGui::SameLine();
		ImGui::InputFloat4("##w", glm::value_ptr(mat4[3]));
	}

	void MyGUIContext::transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
	{
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::DragFloat3("##Position", &position.x);

		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rotation", &rotation.x);

		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::DragFloat3("##Scale", &scale.x);
	}

	//-------------------------------------------------------------
	//
	//	MyDragDropController
	//
	std::string_view MyDragDropController::dragData(const std::filesystem::path& path)
	{
		mIsDragResource = true;
		mFilePath = path;
		mDragName.clear();

		auto ext = mFilePath.filename().extension().string();
		std::transform(ext.begin()
			, ext.end()
			, ext.begin()
			, [](unsigned char c) { return std::tolower(c); });

		mFileType = FileTool::getFileType(ext);
		switch (mFileType)
		{
		case FileType::FT_None:
			break;
		case FileType::FT_Text_Begin:
			break;
		case FileType::FT_Text:
		case FileType::FT_Tysl:
		case FileType::FT_Tyin:
			mDragName = "Drag_Text";
			break;
		case FileType::FT_Text_End:
			break;

			//
		case FileType::FT_Img_Begin:
			break;
		case FileType::FT_Jpg:
		case FileType::FT_Png:
		case FileType::FT_Hdr:
			mDragName = "Drag_Image";
			break;
		case FileType::FT_Img_End:
			break;

			//
		case FileType::FT_Model_Begin:
			break;
		case FileType::FT_Fbx:
		case FileType::FT_Obj:
		case FileType::FT_PMX:
			mDragName = "Drag_Model";
			break;
		case FileType::FT_Model_End:
			break;

			//
		case FileType::FT_Unknown:
			break;
		default:
			break;
		}

		return mDragName;
	}

	std::tuple<bool, std::string> MyDragDropController::dropData()
	{
		bool flag = !mDragName.empty();
		std::string name(mDragName);

		return { flag, mDragName };
	}

	void MyTextureSizeHelper::calculate(const ImVec2& inTextureSize, const ImVec2& inWindowSize, ImVec2& outDisplaySize, ImVec2& outOffsetToCenter, ImVec2& outUV0, ImVec2& outUV1)
	{
		outDisplaySize = inWindowSize;

		//按照视图比例 得到符合当前视图的窗口大小
		float ratio_xDy = inTextureSize.x / inTextureSize.y;
		float ratio_yDx = inTextureSize.y / inTextureSize.x;

		//先保证上下对齐,也就是需要计算左右偏移(例如 600:600 16:9 1.8)
		//获得当前视口大小下,帧图片的理论宽度
		float width = inWindowSize.y * ratio_xDy; // 600*1.8 = 1080
		//如果宽度比当前的小,说明当前视口大小可以容纳当前帧
		//需要把帧图片移动到view中心
		if (width < inWindowSize.x)
		{
			//计算多出来的宽度的一半
			outOffsetToCenter.x = (inWindowSize.x - width) * 0.5f;
			outDisplaySize.x = width;
		}
		else
		{
			//宽度比当前的大,说明当前视口宽度就应该设置成帧图片的最大宽度
			//此时应该计算上下之间预留的空隙
			//以及把帧图片移动到视口正中间
			float height = inWindowSize.x * ratio_yDx;	//600*0.56=337.5
			outOffsetToCenter.y = (inWindowSize.y - height) * 0.5f;
			outDisplaySize.y = height;
		}

		//计算当前显示大小与视图的比值
		//
		outUV0 = ImVec2(0.0f, outDisplaySize.y / inTextureSize.y);
		outUV1 = ImVec2(outDisplaySize.x / inTextureSize.x, 0.0f);

		if (outUV0.y > 1.0f)
		{
			outUV0.y = 1.0f;
		}

		if (outUV1.x > 1.0f)
		{
			outUV1.x = 1.0f;
		}
	}

	void MyTextureSizeHelper::fitImageToRect(const ImVec2& inWindowSize, const ImVec2& inImageSize, ImVec2& outDisplaySize, ImVec2& outOffsetToCenter)
	{
		float target_ratio = inWindowSize.x / inWindowSize.y;
		float image_ratio = inImageSize.x / inImageSize.y;

		// 计算缩放比例
		// 比如 16:9 > 16:10
		if (image_ratio >= target_ratio)
		{
			//此时image的宽超过了target的宽
			//需要用target的宽来计算出缩放后的image的高
			outDisplaySize.x = inWindowSize.x;
			outDisplaySize.y = (inImageSize.y / inImageSize.x) * inWindowSize.x;
			//将image移动的到正中央需要的偏移大小(垂直居中)
			outOffsetToCenter.y = (inWindowSize.y - outDisplaySize.y) * 0.5f; 
		}
		//比如 16:10 < 16:9
		else
		{
			//此时image的高超过了target的高
			//需要用target的高来计算出缩放后的image的宽
			outDisplaySize.y = inWindowSize.y;
			outDisplaySize.x = (inImageSize.x / inImageSize.y) * inWindowSize.y;
			//将image移动的到正中央需要的偏移大小(水平居中)
			outOffsetToCenter.x = (inWindowSize.x - outDisplaySize.x) * 0.5f;
		}
	}
}
