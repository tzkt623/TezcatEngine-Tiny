#pragma once
#include "CommonHead.h"


namespace tezcat::Tiny::Core
{
	class VAO;
	class VAOManager
	{
		CREATE_DELEGATE_INSTANCE(VAOManager)
	public:
		VAOManager();
		~VAOManager();

	public:
		void addVAO(VAO* vao);
		VAO* getVAO(const std::string& name);

	private:
		std::unordered_map<std::string, VAO*> m_VAOWithName;
	};
}