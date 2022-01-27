#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace tezcat::Tiny::Core
{
	class Shader;
	class Texture;
}


namespace tezcat::Tiny::Module
{
	class Material
	{
	public:
		Material(const std::string& name);
		~Material();

	public:
		unsigned int getID() const { return m_ID; }
		Core::Shader* getShader() const { return m_Shader; }

		void addTexture(const std::string& texType, Core::Texture* texture);
		std::unordered_map<std::string, Core::Texture*>& allTexture()
		{
			return m_TextureWithName;
		}

	private:
		std::string m_Name;
		unsigned int m_ID;

		Core::Shader* m_Shader;
		std::unordered_map<std::string, Core::Texture*> m_TextureWithName;
	private:
		static int IDGiver;
	};
}
