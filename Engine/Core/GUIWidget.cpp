#include "GUIWidget.h"
#include "ImGuiHead.h"
#include "GUI.h"

namespace tezcat::Tiny::Core
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
	GUIWidget::GUIWidget(const std::u8string& name) :
		m_Name(Tools::fromU8(name))
	{

	}

	GUIWidget::GUIWidget(const std::string& name) :
		m_Name(name)
	{

	}

	GUIWidget::GUIWidget(const char* name) :
		m_Name(name)
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
		this->onUpdate();
		this->onRender();

		this->beginParent();
		for (auto w : m_Children)
		{
			w->update();
		}
		this->endParent();
	}


	//-------------------------------------------------------
	GUIWindow::GUIWindow(const std::u8string& name) :
		GUIParentWidget(name)
	{

	}

	GUIWindow::GUIWindow(const std::string& name) :
		GUIParentWidget(name)
	{

	}

	GUIWindow::GUIWindow(const char* name) :
		GUIParentWidget(name)
	{

	}

	GUIWindow::~GUIWindow()
	{

	}

	void GUIWindow::beginParent()
	{
		ImGui::Begin(this->getName());
	}

	void GUIWindow::endParent()
	{
		ImGui::End();
	}

	void GUIWindow::open()
	{
		GUI::getInstance()->addWindow(this);
	}

	void GUIWindow::close()
	{
		GUI::getInstance()->removeWindow(this);
	}


	//-------------------------------------------------------------------------
	GUIDragFloat::GUIDragFloat(const std::u8string& name) :
		GUIWidget(name),
		m_Speed(0.1f),
		m_Min(0.0f),
		m_Max(0.0f),
		postFunction(GUIDragFloat::defaultPostFunction)
	{

	}

	GUIDragFloat::~GUIDragFloat()
	{
		postFunction = nullptr;
	}

	//
	GUIDragFloat2::GUIDragFloat2(const std::u8string& name) :
		GUIDragFloat(name),
		m_Data()
	{

	}

	GUIDragFloat2::~GUIDragFloat2()
	{
	}

	void GUIDragFloat2::onRender()
	{
		ImGui::DragFloat2(this->getName(), m_Data, m_Speed, m_Min, m_Max);
		postFunction(m_Data);
	}

	//
	GUIDragFloat3::GUIDragFloat3(const std::u8string& name) :
		GUIDragFloat(name),
		m_Data()
	{

	}

	GUIDragFloat3::~GUIDragFloat3()
	{
	}

	void GUIDragFloat3::onRender()
	{
		ImGui::DragFloat3(this->getName(), m_Data, m_Speed, m_Min, m_Max);
		postFunction(m_Data);
	}


	//------------------------------

	GUIText::GUIText(const std::u8string& name) :
		GUIWidget(name)
	{

	}

	GUIText::~GUIText()
	{

	}


	void GUIText::onRender()
	{
		ImGui::Text(m_Data.c_str());
	}


}