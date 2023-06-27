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
		, mTinyUniformList(UniformID::allStringCount(), UniformInfo{ "", UniformType::Error, -1, -1 })
	{

	}

	Shader::~Shader()
	{
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

	void Shader::setupUserUniformID(UniformType& uniformType, const std::string& name, const int& shaderID)
	{
		if (shaderID < 0)
		{
			return;
		}

		auto info = new UniformInfo{ name, uniformType, shaderID, (int)mUserUniformAry.size() };

		mUserUniformAry.emplace_back(info);
		mUserUniformUMap.try_emplace(info->name, info);
	}

	void Shader::setupTinyUniform(UniformType& uniformType, const std::string& name, const uint32_t& index, const int& shaderID)
	{
		mTinyUniformList[index].name = name;
		mTinyUniformList[index].type = uniformType;
		mTinyUniformList[index].shaderID = shaderID;
		mTinyUniformList[index].index = index;
	}

	void Shader::resizeTinyUniformAry(uint64_t size)
	{
		mTinyUniformList.resize(size, UniformInfo{ "", UniformType::Error, -1 , -1 });
	}

	void Shader::resizeUserUniformAry(uint64_t size)
	{
		mUserUniformUMap.reserve(size);
	}

}

