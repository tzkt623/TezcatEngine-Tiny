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
		, mTinyUniformList(UniformID::allStringCount(), -1)
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

	void Shader::resizeUniformList(uint64_t size, int value)
	{
		mTinyUniformList.resize(size, value);
	}

	void Shader::generate()
	{
		Graphics::getInstance()->cmdCreateShader(this);
	}
}

