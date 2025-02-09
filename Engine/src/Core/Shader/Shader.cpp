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

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Shader, TinyObject)

	Shader::Shader(const std::string& filePath)
		: mProgramID(-1)
		, mName()
		, mPath(filePath)
		, mUID(IDGenerator<Shader, uint32>::generate())
		, mVersion(-1)
		, mOrderID(0)
		, mEnableBlend(false)
		, mEnableZWrite(true)
		, mDepthTest(DepthTest::Less, 0)
		, mRenderQueue(Queue::Opaque)
		, mBlendSource(Blend::One, 0)
		, mBlendTarget(Blend::One, 0)
		, mCullFace(CullFace::Back, 0)
		, mTinyUniformList(UniformID::allStringCount(), nullptr)
	{
		for (uint32 i = 0; i < mTinyUniformList.size(); i++)
		{
			mTinyUniformList[i] = new UniformInfo{ -1, "", UniformType::Error, -1 };
		}

	}

	Shader::~Shader()
	{
		for (auto u : mTinyUniformList)
		{
			delete u;
		}

		for (auto u : mUserUniformAry)
		{
			delete u;
		}

		IDGenerator<Shader, uint32>::recycle(mUID);
	}

	void Shader::apply(uint32 pid)
	{
		mProgramID = pid;
	}

	void Shader::generate()
	{
		mParser = std::make_unique<ShaderParser>();

		this->checkAndLoadContent();

		auto root = std::filesystem::path(mPath).parent_path().string();
		mParser->parseHeader(mContent);
		mParser->parseShaders(mContent, root);
		mParser->updateShaderConfig(this);

		mContent.clear();

		ShaderManager::registerShader(this);
		//Graphics::getInstance()->cmdCreateShader(this);
		
		Graphics::getInstance()->addCommand(new RenderCMD_CreateShader(this));
	}

	void Shader::setupUserUniformID(ArgMetaData* metaData, const std::string& name, const int& shaderID, const int& arrayIndex)
	{
		if (shaderID < 0)
		{
			Log_Warning(fmt::format("{}--{} ID is -1"
				, mPath, name));
		}

		auto member_info = metaData->getInfo<ArgMemberInfo>();

		std::string editor_name;
		if (arrayIndex < 0)
		{
			editor_name = member_info->editorName.empty() ? name : member_info->editorName;
		}
		else
		{
			editor_name = member_info->editorName.empty() ? name : fmt::format("{}[{}]", member_info->editorName, arrayIndex);
		}

		auto info = new UniformInfo
		{
			(int)mUserUniformAry.size()
			, name
			, metaData->valueType
			, shaderID
			, member_info->constraint
			, editor_name
			, member_info->range
		};

		mUserUniformAry.emplace_back(info);
		mUserUniformUMap.try_emplace(info->name, info);
	}

	void Shader::setupTinyUniform(ArgMetaData* metaData, const std::string& name, const uint32& index, const int& shaderID, const int& arrayIndex)
	{
		auto member_info = metaData->getInfo<ArgMemberInfo>();

		std::string editor_name;
		if (arrayIndex < 0)
		{
			editor_name = member_info->editorName.empty() ? name : member_info->editorName;
		}
		else
		{
			editor_name = member_info->editorName.empty() ? name : fmt::format("{}[{}]", member_info->editorName, arrayIndex);
		}

		mTinyUniformList[index]->name = name;
		mTinyUniformList[index]->type = metaData->valueType;
		mTinyUniformList[index]->shaderID = shaderID;
		mTinyUniformList[index]->index = index;
		mTinyUniformList[index]->constraint = member_info->constraint;
		mTinyUniformList[index]->editorName = editor_name;
		mTinyUniformList[index]->range = member_info->range;
	}

	void Shader::resizeTinyUniformAry(uint64 size)
	{
		//mTinyUniformList.resize(size, new UniformInfo{ "", UniformType::Error, -1 , -1 });
	}

	void Shader::resizeUserUniformAry(uint64 size)
	{
		mUserUniformUMap.reserve(size);
	}

	bool Shader::checkAndLoadContent()
	{
		if (mContent.empty())
		{
			mContent = FileTool::loadText(mPath);
			if (mContent.empty())
			{
				return false;
			}
		}

		return true;
	}

}

