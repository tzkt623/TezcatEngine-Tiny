#include "Pass.h"
#include "Shader.h"
#include "RenderObject.h"
#include "Camera.h"
#include "Material.h"
#include "Statistic.h"

namespace tezcat::Tiny::Core
{
	Pass::Pass(Shader* program) :
		m_Shader(program)
	{

	}

	Pass::Pass(Module::Material* material) :
		m_Material(material)
	{
		m_Shader = m_Material->getShader();
	}

	Pass::~Pass()
	{
		m_Material = nullptr;
		m_Shader = nullptr;
	}

	void Pass::render(Module::Camera* camera)
	{
		m_Shader->bind();

		m_Shader->setProjectionMatrix(camera->getProjectionMatrix());
		m_Shader->setViewMatrix(camera->getViewMatrix());

		Statistic::DrawCall = m_ObjectList.size();

		auto it = m_ObjectList.begin();
		while (it != m_ObjectList.end())
		{
			(*it++)->draw(m_Shader);
		}
	}

	int Pass::getID() const
	{
		return m_Shader->getOrderID();
	}

	const std::string& Pass::getName() const
	{
		return m_Shader->getName();
	}

	void Pass::addRenderObject(RenderObject* renderObject)
	{
		m_ObjectList.push_back(renderObject);
	}

}
