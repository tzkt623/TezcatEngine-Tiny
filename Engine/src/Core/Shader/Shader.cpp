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

#include "Core/Shader/Shader.h"
#include "Core/Shader/ShaderParser.h"
#include "Core/Tool/Tool.h"

#include "Core/Manager/ShaderManager.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/RenderCommand.h"

#include "Core/Debug/Debug.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Shader, TinyObject);

	Shader::Shader(const std::string& filePath)
		: mProgramID(0)
		, mName()
		, mPath(filePath)
		, mUID(IDGenerator<Shader, uint32_t>::generate())
		, mVersion(-1)
		, mSortingOffset(0)
		, mEnableBlend(false)
		, mEnableZWrite(true)
		, mDepthTest(DepthTest::Less, 0)
		, mRenderQueue(PipelineQueueType::Opaque)
		, mBlendSource(Blend::One, 0)
		, mBlendTarget(Blend::One, 0)
		, mCullFace(CullFace::Back, 0)
		, mTinyUniformList(UniformID::allStringCount(), nullptr)
	{
		for (uint32_t i = 0; i < mTinyUniformList.size(); i++)
		{
			mTinyUniformList[i] = new UniformValueConfig{ -1, "", UniformType::Error, -1 };
		}
	}

	Shader::~Shader()
	{
		for (auto u : mTinyUniformList)
		{
			delete u;
		}

		for (auto u : mUserUniformValueConfigAry)
		{
			delete u;
		}

		IDGenerator<Shader, uint32_t>::recycle(mUID);
	}

	void Shader::apply(uint32_t pid)
	{
		mProgramID = pid;
	}

	void Shader::generate()
	{
		mParser = std::make_unique<ShaderParser>();

		mParser->parse(mPath);
		mParser->updateShaderConfig(this);
		mContent.reset();

		ShaderManager::registerShader(this);

		Graphics::getInstance()->addCommand<RenderCMD_CreateShader>(this);
	}

	void Shader::setupUserUniformID(ShaderUniformMember* metaData, const std::string& name, const int& shaderID, const int& arrayIndex)
	{
		//if (shaderID < 0)
		//{
		//	TINY_LOG_WARNING(std::format("{}--{} ID is -1", mPath, name));
		//}

		auto member_info = metaData->getInfo<ShaderMemberInfo>();

		std::string editor_name;
		if (arrayIndex < 0)
		{
			editor_name = member_info->editorName.empty() ? name : member_info->editorName;
		}
		else
		{
			editor_name = member_info->editorName.empty() ? name : std::format("{}[{}]", member_info->editorName, arrayIndex);
		}

		auto info = new UniformValueConfig
		{
			(int)mUserUniformValueConfigAry.size()
			, name
			, metaData->valueType
			, shaderID
			, member_info->constraint
			, editor_name
			, member_info->range
		};

		mUserUniformValueConfigAry.emplace_back(info);
		mUserUniformValueConfigMap.try_emplace(info->name, info);
	}

	void Shader::setupTinyUniform(ShaderUniformMember* metaData, const std::string& name, const uint32_t& index, const int& shaderID, const int& arrayIndex)
	{
		auto member_info = metaData->getInfo<ShaderMemberInfo>();

		std::string editor_name;
		if (arrayIndex < 0)
		{
			editor_name = member_info->editorName.empty() ? name : member_info->editorName;
		}
		else
		{
			editor_name = member_info->editorName.empty() ? name : std::format("{}[{}]", member_info->editorName, arrayIndex);
		}

		mTinyUniformList[index]->name = name;
		mTinyUniformList[index]->type = metaData->valueType;
		mTinyUniformList[index]->valueID = shaderID;
		mTinyUniformList[index]->index = index;
		mTinyUniformList[index]->constraint = member_info->constraint;
		mTinyUniformList[index]->editorName = editor_name;
		mTinyUniformList[index]->range = member_info->range;
	}

	void Shader::resizeUserUniformArray(uint64_t size)
	{
		mUserUniformValueConfigMap.reserve(size);
	}

	bool Shader::checkAndLoadContent()
	{
		if (!mContent)
		{
			mContent = std::make_unique<std::string>(std::move(FileTool::loadText(mPath)));
			if (mContent->empty())
			{
				return false;
			}
		}

		return true;
	}

	void Shader::rebuild()
	{
		mParser = std::make_unique<ShaderParser>();
		mParser->parse(*mContent.get(), mPath);
		mContent->clear();
		mContent.reset();
	}

	std::string Shader::getMemoryInfo()
	{
		return TINY_OBJECT_MEMORY_INFO();
	}

	void Shader::registerTinyUniform(ShaderUniformMember* memberData)
	{
		this->parseTinyUniform(memberData, "");
	}

	void Shader::parseTinyUniform(ShaderUniformMember* memberData, const std::string& parentName)
	{
		auto& name = memberData->valueName;
		auto array_size = memberData->valueCount;
		auto is_root = parentName.empty();

		//如果是类,需要拼接名称
		if (memberData->valueType == UniformType::Struct)
		{
			auto& members = memberData->getInfo<ShaderStructInfo>()->members;

			if (array_size > 0)
			{
				for (int32_t i = 0; i < array_size; i++)
				{
					for (auto& m : members)
					{
						std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
						this->parseTinyUniform(m.get(), true_name);
					}
				}
			}
			else
			{
				for (auto& m : members)
				{
					std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
					this->parseTinyUniform(m.get(), true_name);
				}
			}
		}
		else
		{
			if (array_size > 0)
			{
				for (int32_t i = 0; i < array_size; i++)
				{
					std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
					auto uid = UniformID::staticGetUID(true_name);
					if (uid < this->getTinyUniformCount())
					{
						this->setupTinyUniform(memberData, true_name, uid, -1, i);
					}
					else
					{
						TINY_LOG_ERROR(std::format("This TinyUniform is not register [{}]", true_name));
					}
				}
			}
			else
			{
				std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
				auto uid = UniformID::staticGetUID(true_name);
				if (uid < this->getTinyUniformCount())
				{
					this->setupTinyUniform(memberData, true_name, uid, -1);
				}
				else
				{
					TINY_LOG_ERROR(std::format("This TinyUniform is not register [{}]", true_name));
				}
			}
		}
	}

	void Shader::registerUserUniform(ShaderUniformMember* memberData)
	{
		this->parseUserUniform(memberData, "");
	}

	void Shader::parseUserUniform(ShaderUniformMember* memberData, const std::string& parentName)
	{
		auto& name = memberData->valueName;
		auto array_size = memberData->valueCount;
		auto is_root = parentName.empty();

		//如果是类,需要拼接名称
		if (memberData->valueType == UniformType::Struct)
		{
			auto& members = memberData->getInfo<ShaderStructInfo>()->members;

			if (array_size > 0)
			{
				for (int32_t i = 0; i < array_size; i++)
				{
					for (auto& m : members)
					{
						std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
						this->parseUserUniform(m.get(), true_name);
					}
				}
			}
			else
			{
				for (auto& m : members)
				{
					std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
					this->parseUserUniform(m.get(), true_name);
				}
			}
		}
		else
		{
			if (array_size > 0)
			{
				for (int32_t i = 0; i < array_size; i++)
				{
					std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
					this->setupUserUniformID(memberData, true_name, -1, i);
				}
			}
			else
			{
				std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
				this->setupUserUniformID(memberData, true_name, -1);
			}
		}
	}

	void Shader::setTinyUniform(const std::string& name, const int32_t& shaderID)
	{
		auto uid = UniformID::staticGetUID(name);
		if (uid < this->getTinyUniformCount())
		{
			mTinyUniformList[uid]->valueID = shaderID;
		}
		else
		{
			TINY_LOG_ERROR(std::format("This TinyUniform is not register [{}]", name));
		}
	}

	void Shader::setUserUniform(const std::string& name, const int32_t& shaderID)
	{
		auto it = mUserUniformValueConfigMap.find(name);
		if (it != mUserUniformValueConfigMap.end())
		{
			it->second->valueID = shaderID;
		}
		else
		{
			TINY_LOG_ERROR(std::format("This TinyUniform is not register [{}]", name));
		}
	}
}

