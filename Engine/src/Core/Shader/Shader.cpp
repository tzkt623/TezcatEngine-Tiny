/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

		RenderCommandHelper::addCommand<RenderCMD_CreateShader>(this);
	}

	void Shader::setupUserUniformID(ShaderMetaDataMember* metaData, const std::string& name, const int& shaderID, const int& arrayIndex)
	{
		//if (shaderID < 0)
		//{
		//	TINY_LOG_WARNING(std::format("{}--{} ID is -1", mPath, name));
		//}

		//auto member_info = metaData->getInfo<ShaderMemberInfo>();

		auto attribute = metaData->attributeEditor.get();

		//std::string editor_name;
		//if (arrayIndex < 0)
		//{
		//	editor_name = attribute->editorName.empty() ? name : attribute->editorName;
		//}
		//else
		//{
		//	editor_name = attribute->editorName.empty() ? name : std::format("{}[{}]", attribute->editorName, arrayIndex);
		//}

		auto info = new UniformValueConfig
		{
			(int)mUserUniformValueConfigAry.size()
			, name
			, metaData->valueType
			, shaderID
			, ""
			, metaData->attributeEditor.get()
			, metaData->attributeRegister.get()
		};

		info->editorName = attribute ? metaData->attributeEditor->editorName : info->name;

		mUserUniformValueConfigAry.emplace_back(info);
		mUserUniformValueConfigMap.try_emplace(info->name, info);
	}

	void Shader::setupTinyUniform(ShaderMetaDataMember* metaData, const std::string& name, const uint32_t& index, const int& shaderID, const int& arrayIndex)
	{
		//auto member_info = metaData->getInfo<ShaderMemberInfo>();
		//auto attribute = metaData->attributeEditor.get();

		//std::string editor_name;
		//if (arrayIndex < 0)
		//{
		//	editor_name = attribute->editorName.empty() ? name : attribute->editorName;
		//}
		//else
		//{
		//	editor_name = attribute->editorName.empty() ? name : std::format("{}[{}]", attribute->editorName, arrayIndex);
		//}

		auto attribute = metaData->attributeEditor.get();

		mTinyUniformList[index]->name = name;
		mTinyUniformList[index]->type = metaData->valueType;
		mTinyUniformList[index]->valueID = shaderID;
		mTinyUniformList[index]->index = index;
		mTinyUniformList[index]->editorName = attribute ? metaData->attributeEditor->editorName : mTinyUniformList[index]->name;
		mTinyUniformList[index]->attributeEditor = metaData->attributeEditor.get();
		mTinyUniformList[index]->attributeRegister = metaData->attributeRegister.get();
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

	void Shader::registerGlobalUniform(ShaderMetaDataArgument* memberData)
	{

	}

	void Shader::registerTinyUniform(ShaderMetaDataArgument* memberData)
	{
		this->parseTinyUniform(memberData, "");
	}

	void Shader::parseTinyUniform(ShaderMetaDataArgument* argument, const std::string& parentName)
	{
		auto& name = argument->name;
		auto& array_size = argument->arrayCount;
		auto is_root = parentName.empty();
		std::string true_name;

		//如果是类,需要拼接名称
		if (argument->valueType == UniformType::Struct)
		{
			auto _struct = (ShaderMetaDataStruct*)argument;
			auto& members = _struct->getMembers();

			if (array_size > 0)
			{
				for (uint32_t i = 0; i < array_size; i++)
				{
					for (auto& member : members)
					{
						true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
						this->parseTinyUniform(member.get(), true_name);
					}
				}
			}
			else
			{
				for (auto& member : members)
				{
					//如果父名称为空,说明当前是声明的类变量
					//就把类变量名传入下一层,组合成成员变量的形式,即TinyXXX.xxxxx
					//如果父名称不为空,说明是类成员变量,需要先组合再继续传
					true_name = is_root ? name : std::format("{}.{}", parentName, name);
					this->parseTinyUniform(member.get(), true_name);
				}
			}
		}
		else
		{
			auto member = (ShaderMetaDataMember*)argument;

			if (array_size > 0)
			{
				for (uint32_t i = 0; i < array_size; i++)
				{
					true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
					auto uid = UniformID::staticGetUID(true_name);
					if (uid < this->getTinyUniformCount())
					{
						this->setupTinyUniform(member, true_name, uid, -1, i);
					}
					else
					{
						TINY_LOG_ERROR(std::format("This TinyUniform is not register [{}]", true_name));
					}
				}
			}
			else
			{
				//如果父名称为空
				//说明当前就是一个需要被设置的成员变量了,不管他是类中的,还是独立的
				//他的名称就等于name
				//如果父名称不为空
				//说明需要组合以后再设置
				true_name = is_root ? name : std::format("{}.{}", parentName, name);
				auto uid = UniformID::staticGetUID(true_name);
				if (uid < this->getTinyUniformCount())
				{
					this->setupTinyUniform(member, true_name, uid, -1);
				}
				else
				{
					TINY_LOG_ERROR(std::format("This TinyUniform is not register [{}]", name));
				}
			}
		}
	}

	void Shader::registerUserUniform(ShaderMetaDataArgument* memberData)
	{
		this->parseUserUniform(memberData, "");
	}

	void Shader::parseUserUniform(ShaderMetaDataArgument* argument, const std::string& parentName)
	{
		auto& name = argument->name;
		auto& array_size = argument->arrayCount;
		auto is_root = parentName.empty();

		//如果是类,需要拼接名称
		if (argument->valueType == UniformType::Struct)
		{
			//auto& members = memberData->getInfo<ShaderStructInfo>()->members;

			auto _struct = (ShaderMetaDataStruct*)argument;
			auto& members = _struct->getMembers();

			if (array_size > 0)
			{
				for (uint32_t i = 0; i < array_size; i++)
				{
					for (auto& member : members)
					{
						std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
						this->parseUserUniform(member.get(), true_name);
					}
				}
			}
			else
			{
				for (auto& member : members)
				{
					std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
					this->parseUserUniform(member.get(), true_name);
				}
			}
		}
		else
		{
			auto member = (ShaderMetaDataMember*)argument;

			if (array_size > 0)
			{
				for (uint32_t i = 0; i < array_size; i++)
				{
					std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
					this->setupUserUniformID(member, true_name, -1, i);
				}
			}
			else
			{
				std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
				this->setupUserUniformID(member, true_name, -1);
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

