#include "MyTextureViewerWindow.h"

namespace tezcat::Editor
{
	MyTextureViewerWindow::MyTextureViewerWindow(const std::string& name)
		: GUIWindow(name)
		, mTex(nullptr)
		, mSize(800, 600)
	{

	}

	MyTextureViewerWindow::~MyTextureViewerWindow()
	{
		mTex->deleteObject();
	}

	void MyTextureViewerWindow::setTexture(Texture* tex)
	{
		mTex = tex;
		mTex->saveObject();
	}

	void MyTextureViewerWindow::loadTexture(const std::filesystem::path& path)
	{
		if (mTex)
		{
			mTex->deleteObject();
			mTex = nullptr;
		}

		Image* img = Image::create();
		img->openFile(path.string(), true);

		Texture2D* t2d = Texture2D::create(path.filename().string());
		t2d->setImage(img);
		t2d->generate();

		mTex = t2d;
		mTex->saveObject();
	}

	bool MyTextureViewerWindow::begin()
	{
		return ImGui::Begin(mName.c_str(), &mIsOpen, ImGuiWindowFlags_NoScrollbar);
	}

	void MyTextureViewerWindow::onRender()
	{
		GUIWindow::onRender();
		ImGui::SetNextItemWidth(128);
		ImGui::InputFloat2("WindowSize", &mSize.x);

		ImGui::SetWindowSize(mSize, ImGuiCond_FirstUseEver);
		mSize = ImGui::GetWindowSize();

		if (mTex)
		{
			switch (mTex->getTextureType())
			{
			case TextureType::Texture2D:
			{
				ImGui::BeginChild("##Viewport");
				auto t2d = (Texture2D*)mTex;
				float img_width = t2d->getWidth();
				float img_hegiht = t2d->getHeight();

				auto cw_size = ImGui::GetWindowSize();
				ImVec2 img_item_size = cw_size;
				ImVec2 img_item_offset;

				//以X轴为第一对齐轴
				float rate_yDx = img_hegiht / img_width;

				//假设X轴顶满
				//以X轴的width为基准,算出图片的height
				auto height = cw_size.x * rate_yDx;
				//如果图片的height小于窗口的y
				//说明假设成立
				if (height < cw_size.y)
				{
					//计算出Y轴上的便宜把图片放到中心位置
					img_item_size.y = height;
					img_item_offset.y = (cw_size.y - img_item_size.y) * 0.5f;
				}
				//如果假设不成立
				//说明图片需要更大的height才能按比例显示
				//所以当前应该以窗口的height为基础计算图片的width
				else
				{
					//Y轴顶满,用Y轴算图片的width
					//并且算出X轴的偏移把图片放到中心
					float rate_xDy = img_width / img_hegiht;
					auto width = cw_size.y * rate_xDy;
					img_item_size.x = width;
					img_item_offset.x = (cw_size.x - img_item_size.x) * 0.5f;
				}

				ImGui::SetCursorPos(img_item_offset);
				ImGui::Image((ImTextureID)t2d->getTextureID()
				   , img_item_size
				   , ImVec2(0, 1)
				   , ImVec2(1, 0));
				ImGui::EndChild();
				break;
			}
			case TextureType::TextureCube:
				break;
			default:
				break;
			}
		}
	}
}
