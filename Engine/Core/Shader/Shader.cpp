#include "Shader.h"
#include "../Manager/PipelineManager.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	Shader::Shader()
		: Shader("##ErrorShader", 0)
	{

	}

	Shader::Shader(const std::string& name, int orderID)
		: m_ProgramID(-1)
		, m_Name(name)
		, m_UID(IDGenerator<Shader, uint32_t>::generate())
		, m_OrderID(orderID)
		, m_ViewMatrixID(0)
		, m_ModelMatrixID(0)
		, m_ProjectionMatrixID(0)
		, m_NormalMatrixID(0)
		, m_ViewPositionID(0)
		, m_EnableBlend(false)
		, m_EnableCullFace(true)
		, m_EnableZWrite(true)
		, m_EnableLighting(true)
		, m_Version(-1)
		, m_RenderQueue(Pipeline::Opaque)
	{
	}

	Shader::~Shader()
	{

	}


	void Shader::apply()
	{
		this->onApply();
		PipelineManager::createPass(this);
	}
}

