#pragma once
#include "../Head/CppHead.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class ShaderPackage;
	class Texture;
	class Material
	{
	public:
		Material(const std::string& name);
		~Material();

	public:
		int getUID();
		ShaderPackage* getShaderPackage() const { return m_ShaderPackage; }

		void addTexture(const std::string& texType, Texture* texture);
		std::unordered_map<std::string, Texture*>& allTexture() { return m_TextureWithName; }

	private:
		std::string m_Name;
		ShaderPackage* m_ShaderPackage;
		std::unordered_map<std::string, Texture*> m_TextureWithName;
	private:
		static int IDGiver;
	};
}
