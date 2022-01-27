#pragma once
#include <string>
#include <vector>

namespace tezcat::Tiny::Core { class Shader; }


namespace tezcat::Tiny::Module
{
	class Material
	{
	public:
		Material(const std::string& name);
		~Material();

	public:
		int getID() const { return m_ID; }
		Core::Shader* getShader() const { return m_Shader; }

	private:
		std::string m_Name;
		int m_ID;
		Core::Shader* m_Shader;
	private:
		static int IDGiver;

	private:
		std::vector<void*> m_Texture;
	};
}
