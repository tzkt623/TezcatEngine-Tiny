#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class PassShader;
	class TINY_API PipelineManager
	{
	public:
		static void createPass(Shader* shader);
		static PassShader* getPass(int uid) { return m_PassList[uid]; }

	private:
		static std::vector<PassShader*> m_PassList;
	};
}