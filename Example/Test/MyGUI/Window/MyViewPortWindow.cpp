#include "MyViewPortWindow.h"




MyViewPortWindow::MyViewPortWindow()
	: GUIWindow("Viewport")
	, mColorBuffer(nullptr)
{

}

MyViewPortWindow::~MyViewPortWindow()
{

}

void MyViewPortWindow::onUpdate()
{

}

void MyViewPortWindow::calculate(const ImVec2& inTextureSize, const ImVec2& inWindowSize
	, ImVec2& outDisplaySize, ImVec2& outOffset, ImVec2& outUV0, ImVec2& outUV1)
{
	outDisplaySize = inWindowSize;

	//按照视图比例 得到符合当前视图的诊室窗口大小
	float ratio_x = inTextureSize.x / inTextureSize.y;
	float ratio_y = inTextureSize.y / inTextureSize.x;

	//先保证上下对齐,也就是需要计算左右偏移
	//获得当前比例下的宽度
	float width = outDisplaySize.y * ratio_x;
	//如果宽度比当前的小,需要把图片移动到view中心
	if (width < outDisplaySize.x)
	{
		//计算多出来的宽度的一半
		outOffset.x = (outDisplaySize.x - width) * 0.5f;
		outDisplaySize.x = outDisplaySize.y * ratio_x;
	}
	else
	{
		//宽度比当前的大
		//左右压缩满了
		//计算上下之间的空隙
		float height = outDisplaySize.x * ratio_y;
		outOffset.y = (outDisplaySize.y - height) * 0.5f;
		outDisplaySize.y = outDisplaySize.x * ratio_y;
	}

	//计算当前显示大小于视图的比值
	//
	outUV0 = ImVec2(0, outDisplaySize.y / inTextureSize.y);
	outUV1 = ImVec2(outDisplaySize.x / inTextureSize.x, 0);

	if (outUV0.y > 1)
	{
		outUV0.y = 1;
	}

	if (outUV1.x > 1)
	{
		outUV1.x = 1;
	}
}

void MyViewPortWindow::onRender()
{
	if (mColorBuffer == nullptr)
	{
		mColorBuffer = (TextureBuffer2D*)TextureMgr::getInstance()->findTexture("RB_World1");
	}

	if (mColorBuffer)
	{
		ImVec2 texture_size = ImVec2(Engine::getScreenWidth(), Engine::getScreenHeight());
		ImVec2 display_size, offset, uv0, uv1;

		this->calculate(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);

		CameraMgr::getInstance()->getMainCamera()->setViewRect(0, 0, display_size.x, display_size.y);

		ImGui::SetCursorPos(offset);
		ImGui::Image((ImTextureID)mColorBuffer->getTextureID()
			, display_size
			, uv0
			, uv1);
	}
}
