#pragma once
#include <memory>
#include <list>
#include <string>
#include <unordered_map>

namespace tezcat::Tiny::Module
{
	class Camera;
	class Material;
}

namespace tezcat::Tiny::Core
{
	class RenderAgent;
	class Shader;
	class VAO;
	class Pass_VAO;


	/// <summary>
	/// Pass是由Shader来生成的
	/// 使用同一个Shader的Object会被放在一起渲染
	/// </summary>
	class Pass_Shader
	{
	public:
		Pass_Shader(Shader* program);
		Pass_Shader(Module::Material* material);
		~Pass_Shader();

		void addRenderAgent(RenderAgent* renderAgent);
	public:
		void render(Module::Camera* camera);

		int getOrderID() const;
		int getProgramID() const;
		const std::string& getName() const;

	private:
		Shader* m_Shader;
		std::list<RenderAgent*> m_AgentList;
		std::unordered_map<VAO*, Pass_VAO*> m_AgentWithID;
	};


}

