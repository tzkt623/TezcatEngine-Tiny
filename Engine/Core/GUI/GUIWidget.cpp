#include "GUIWidget.h"
#include "../Tool/Tool.h"
#include "GUI.h"

namespace tezcat::Tiny
{
	float GUIFunction::getFrameRate()
	{
		return ImGui::GetIO().Framerate;
	}

	float GUIFunction::getFrameRateTime()
	{
		return 1000.0f / ImGui::GetIO().Framerate;
	}

	//----------------------------------------------------------------------
	GUIWidget::GUIWidget(const std::u8string& name)
		: mName(StringTool::fromU8(name))
	{

	}

	GUIWidget::GUIWidget(const std::string& name)
		: mName(name)
	{

	}

	GUIWidget::GUIWidget(const char* name)
		: mName(name)
	{

	}

	GUIWidget::~GUIWidget()
	{

	}

	void GUIWidget::update()
	{
		this->onUpdate();
		this->onRender();
	}


	//-------------------------------------------------------
	void GUIParentWidget::onRender()
	{

	}

	void GUIParentWidget::update()
	{
		this->begin();

		this->onUpdate();
		this->onRender();

		this->end();
	}


	//-------------------------------------------------------
	GUIWindow::GUIWindow(const std::u8string& name)
		: GUIParentWidget(name)
	{

	}

	GUIWindow::GUIWindow(const std::string& name)
		: GUIParentWidget(name)
	{

	}

	GUIWindow::GUIWindow(const char* name)
		: GUIParentWidget(name)
	{

	}

	GUIWindow::~GUIWindow()
	{

	}

	void GUIWindow::begin()
	{
		ImGui::Begin(this->getName(), 0);
	}

	void GUIWindow::end()
	{
		ImGui::End();
	}

	void GUIWindow::open(GUI* gui)
	{
		mHost = gui;
		mHost->addWindow(this);
	}

	void GUIWindow::close()
	{
		mHost->removeWindow(this);
	}


	//-------------------------------------------------------------------------
	GUIDragFloat::GUIDragFloat(const std::u8string& name)
		: GUIWidget(name)
		, m_Speed(0.1f)
		, m_Min(0.0f)
		, m_Max(0.0f)
		, postFunction(GUIDragFloat::defaultPostFunction)
	{

	}

	GUIDragFloat::~GUIDragFloat()
	{
		postFunction = nullptr;
	}

	//----------------------------------------------------------
	GUIDragFloat1::GUIDragFloat1(const std::u8string& name)
		: GUIDragFloat(name)
		, mData(0)
	{

	}

	GUIDragFloat1::~GUIDragFloat1()
	{

	}

	void GUIDragFloat1::onRender()
	{
		ImGui::DragFloat(this->getName(), &mData, m_Speed, m_Min, m_Max);
		postFunction(&mData);
	}

	//
	GUIDragFloat2::GUIDragFloat2(const std::u8string& name)
		: GUIDragFloat(name)
		, mData()
	{
		mData[0] = 0;
		mData[1] = 0;
	}

	GUIDragFloat2::~GUIDragFloat2()
	{
	}

	void GUIDragFloat2::onRender()
	{
		ImGui::DragFloat2(this->getName(), mData, m_Speed, m_Min, m_Max);
		postFunction(mData);
	}

	//
	GUIDragFloat3::GUIDragFloat3(const std::u8string& name) :
		GUIDragFloat(name),
		mData()
	{
		mData[0] = 0;
		mData[1] = 0;
		mData[2] = 0;
	}

	GUIDragFloat3::~GUIDragFloat3()
	{
	}

	void GUIDragFloat3::onRender()
	{
		ImGui::DragFloat3(this->getName(), mData, m_Speed, m_Min, m_Max);
		postFunction(mData);
	}


	//------------------------------

	GUIText::GUIText(const std::u8string& name)
		: GUIWidget(name)
	{

	}

	GUIText::~GUIText()
	{

	}


	void GUIText::onRender()
	{
		ImGui::Text(mData.c_str());
	}

	//------------------------------
	GUIImage::GUIImage(const std::u8string& name)
		: GUIWidget(name)
	{

	}


	void GUIImage::onRender()
	{

	}

	void GUIImage::onUpdate()
	{
	}

	GUIImage::~GUIImage()
	{

	}

	void GUIImage::refresh()
	{

	}


	GUIMatrix4::GUIMatrix4(const std::u8string& name)
		: GUIWidget(name)
	{

	}

	GUIMatrix4::~GUIMatrix4()
	{

	}

	void GUIMatrix4::onRender()
	{
		ImGui::Text(this->getName());
		auto& d = *mData;
		ImGui::InputFloat4("x", glm::value_ptr(d[0]));
		ImGui::InputFloat4("y", glm::value_ptr(d[1]));
		ImGui::InputFloat4("z", glm::value_ptr(d[2]));
		ImGui::InputFloat4("w", glm::value_ptr(d[3]));
	}

}
