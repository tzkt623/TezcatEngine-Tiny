#pragma once
#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Shader/ShaderParam.h"
#include "../Shader/Uniform.h"
#include "../Shader/Shader.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Shader;
	class TINY_API Material : public TinyObject
	{
		Material(std::string name);
		Material(Shader* shader);
		TINY_OBJECT_H(Material, TinyObject)

	public:
		virtual ~Material();

	public:
		std::string& getName() { return mName; }
		std::string_view getNameView() { return mName; }

		int32 getUID() const;
		std::vector<Uniform*>& getUniforms() { return mUniforms; }
		Shader* getShader() const { return mShader; }
		void setShader(Shader* shader);

	public:
		template<typename UniformType, typename... Args>
		void setTinyUniform(const UniformID& uniformID, Args&&... value)
		{
			auto shader_id = mShader->getTinyUniformShaderID(uniformID);

			auto result = std::ranges::find_if(mUniforms, [&](Uniform* uniform)
			{
				return *uniform->mShaderID == shader_id;
			});

			if (result != mUniforms.end())
			{
				static_cast<UniformType*>(*result)->set(std::forward<Args>(value)...);
			}
			else
			{
				TINY_THROW_LOGIC(StringTool::stringFormat("Material : This uniform [%s] not found!", uniformID.getStringData()));
			}
		}

		template<typename UniformType, typename ValueType>
		void setUniform(const std::string& name, ValueType&& value)
		{
			auto id = mShader->getUniformIndex(name);
			if (id != -1)
			{
				static_cast<UniformType*>(mUniforms[id])->set(std::forward<ValueType>(value));
			}
			else
			{
				TINY_THROW_LOGIC(StringTool::stringFormat("Material : This uniform [%s] not found!", name.c_str()));
			}
		}

		template<typename UniformType, typename ValueType>
		void setUniform(const uint32& id, ValueType&& value)
		{
			static_cast<UniformType*>(mUniforms[id])->set(std::forward<ValueType>(value));
		}

		template<typename UniformType, typename ValueType>
		void setUniform(const uint32& id, ValueType& value)
		{
			static_cast<UniformType*>(mUniforms[id])->set(value);
		}

		void submit(Shader* shader);

	private:
		std::string mName;
		std::vector<Uniform*> mUniforms;

	private:
		Shader* mShader;
	};
}
