#include "VAOManager.h"
#include "VAO.h"

namespace tezcat::Tiny::Core
{
	IMPLEMENT_DELEGATE_INSTANCE(VAOManager);
	VAOManager::VAOManager()
	{
		REGISTER_DELEGATE_INSTANCE(VAOManager);
	}

	VAOManager::~VAOManager()
	{

	}

	void VAOManager::addVAO(VAO* vao)
	{
		m_VAOWithName.emplace(vao->getName(), vao);
	}

	VAO* VAOManager::getVAO(const std::string& name)
	{
		return m_VAOWithName[name];
	}
}
