#pragma once
#include "../Head/CppHead.h"
#include "../Shader/ShaderParam.h"
#include "../Shader/Uniform.h"

namespace tezcat::Tiny::Core
{
	struct Uniform;
	class ShaderPackage;
	class Texture;
	class Shader;
	class Transform;
	class Material
	{
	public:
		Material(const std::string& name);
		~Material();

	public:
		int getUID();
		ShaderPackage* getShaderPackage() const { return m_ShaderPackage; }
		std::vector<ref<Uniform>>& getUniforms() { return m_Uniforms; }

	public:
		template<typename UniformType, typename... Args>
		ref<Uniform> addUniform(const UniformID& uniformID, Args&&... value)
		{
			return m_Uniforms.emplace_back(new UniformType(uniformID, std::forward<Args>(value)...));
		}

		template<typename UniformType, typename Args>
		void setUniform(const UniformID& uniformID, Args&& value)
		{
			auto result = std::find(m_Uniforms.begin(), m_Uniforms.end(),
				[&](Uniform* uniform)
				{
					return uniform->ID == uniformID;
				});

			if (result != m_Uniforms.end())
			{
				result->value = std::forward<Args>(value);
			}
			else
			{
				throw std::logic_error(Tools::stringFormat("Material : This uniform [%s] not found!", uniformID.getStringData()));
				//				m_Uniforms.push_back(new UniformType(uniformID, std::forward<Args>(value)...));
			}
		}

		void submit(Transform* transform, Shader* shader);

	private:
		std::string m_Name;
		ShaderPackage* m_ShaderPackage;
		std::vector<ref<Uniform>> m_Uniforms;
	};
}
