#pragma once

/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
		TINY_OBJECT_H(Material, TinyObject)
	private:
		Material(std::string name);
		Material(Shader* shader);

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
				return *uniform->mValueID == shader_id;
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
		void setUniform(const uint32_t& id, ValueType&& value)
		{
			static_cast<UniformType*>(mUniforms[id])->set(TINY_FWD(value));
		}

		template<typename UniformType, typename ValueType>
		void setUniform(const uint32_t& id, ValueType& value)
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
