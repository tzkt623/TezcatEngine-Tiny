#include "Shader.h"
#include "../Tool/Tool.h"


#include "../Manager/ShaderManager.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/BaseGraphics.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Shader);
	Shader::Shader(const std::string& filePath)
		: mProgramID(-1)
		, mName()
		, mPath(filePath)
		, mUID(IDGenerator<Shader, uint32_t>::generate())
		, mOrderID(0)
		, mEnableBlend(false)
		, mDepthTest(DepthTest::Less, 0)
		, mEnableZWrite(true)
		, mVersion(-1)
		, mRenderQueue(Queue::Opaque)
		, mBlendSource(Blend::One, 0)
		, mBlendTarget(Blend::One, 0)
		, mCullFace(CullFace::Back, 0)
		, mTinyUniformList(UniformID::allStringCount(), nullptr)
	{
		for (uint32_t i = 0; i < mTinyUniformList.size(); i++)
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

		IDGenerator<Shader, uint32_t>::recycle(mUID);
	}

	void Shader::apply(uint32_t pid)
	{
		mProgramID = pid;
		ShaderMgr::getInstance()->registerShader(this);
	}

	void Shader::generate()
	{
		Graphics::getInstance()->cmdCreateShader(this);
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

	void Shader::setupTinyUniform(ArgMetaData* metaData, const std::string& name, const uint32_t& index, const int& shaderID, const int& arrayIndex)
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

	void Shader::resizeTinyUniformAry(uint64_t size)
	{
		//mTinyUniformList.resize(size, new UniformInfo{ "", UniformType::Error, -1 , -1 });
	}

	void Shader::resizeUserUniformAry(uint64_t size)
	{
		mUserUniformUMap.reserve(size);
	}

}

