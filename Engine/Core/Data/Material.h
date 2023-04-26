#pragma once
#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Shader/ShaderParam.h"
#include "../Shader/Uniform.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	struct Uniform;
	class ShaderPackage;
	class Shader;
	class Transform;
	class TINY_API Material : public TinyObject
	{
		Material(const std::string& name);
		TINY_Factory(Material)
		TINY_RTTI_H(Material)

	public:
		virtual ~Material();

	public:
		int getUID() const;
		ShaderPackage* getShaderPackage() const { return mShaderPackage; }
		std::vector<Uniform*>& getUniforms() { return mUniforms; }

	public:
		template<typename UniformType, typename... Args>
		UniformType* addUniform(const UniformID& uniformID, Args&&... value)
		{
			return (UniformType*)mUniforms.emplace_back(new UniformType(uniformID, std::forward<Args>(value)...));
		}

		template<typename UniformType, typename Args>
		void setUniform(const UniformID& uniformID, Args&& value)
		{
			auto result = std::find(mUniforms.begin(), mUniforms.end(),
				[&](Uniform* uniform)
				{
					return uniform->ID == uniformID;
				});

			if (result != mUniforms.end())
			{
				result->value = std::forward<Args>(value);
			}
			else
			{
				throw std::logic_error(StringTool::stringFormat("Material : This uniform [%s] not found!", uniformID.getStringData()));
				//				mUniforms.push_back(new UniformType(uniformID, std::forward<Args>(value)...));
			}
		}

		void submit(Transform* transform, Shader* shader);

	private:
		std::string mName;
		std::vector<Uniform*> mUniforms;
		std::unordered_map<std::string, Uniform*> mUniformsMap;

	private:
		ShaderPackage* mShaderPackage;
	};
}
