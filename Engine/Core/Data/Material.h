#pragma once
#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Shader/ShaderParam.h"
#include "../Shader/Uniform.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class ShaderPackage;
	class Shader;
	class TINY_API Material : public TinyObject
	{
		Material(const std::string& name);
		TINY_Factory(Material);
		TINY_RTTI_H(Material);

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
			auto result = std::ranges::find_if(mUniforms, [&](Uniform* uniform)
				{
					return uniform->ID == uniformID;
				});

			if (result != mUniforms.end())
			{
				static_cast<UniformType*>(*result)->value = std::forward<Args>(value);
			}
			else
			{
				TinyThrow_Logic(StringTool::stringFormat("Material : This uniform [%s] not found!", uniformID.getStringData()));
			}
		}

		void submit(Shader* shader);

	private:
		std::string mName;
		std::vector<Uniform*> mUniforms;

	private:
		ShaderPackage* mShaderPackage;
	};
}
