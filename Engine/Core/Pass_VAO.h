#pragma once
#include <queue>
#include <list>
#include <string>

namespace tezcat::Tiny::Core
{
	class RenderAgent;
	class Shader;
	class VAO;
	class Pass_VAO
	{
	public:
		Pass_VAO(VAO* vao);
		~Pass_VAO();
		void addRenderAgent(RenderAgent* renderAgent);
		void render(Shader* shader);
		bool empty() const { return m_AgentList.empty(); }
	private:
		VAO* m_DelegateVAO;
		std::list<RenderAgent*> m_AgentList;
	};
}