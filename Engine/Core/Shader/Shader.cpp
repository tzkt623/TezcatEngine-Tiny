#include "Shader.h"
#include "Utility/Utility.h"

#include "../Manager/ShaderManager.h"
#include "../Renderer/RenderPass.h"

namespace tezcat::Tiny::Core
{
	Shader::Shader()
		: Shader("##ErrorShader", 0)
	{

	}

	Shader::Shader(const std::string& name, int orderID)
		: mProgramID(-1)
		, mName(name)
		, mUID(IDGenerator<Shader, uint32_t>::generate())
		, mOrderID(orderID)
		, mViewMatrixID(0)
		, mModelMatrixID(0)
		, mProjectionMatrixID(0)
		, mNormalMatrixID(0)
		, mViewPositionID(0)
		, mEnableBlend(false)
		, mDepthTest(DepthTest::Less, 0)
		, mEnableZWrite(true)
		, mEnableLighting(true)
		, mVersion(-1)
		, mRenderQueue(PipelineQueue::Geometry)
		, mBlendSource(Blend::One, 0)
		, mBlendTarget(Blend::One, 0)
		, mCullFace(CullFace::Back, 0)
		, mTinyUniformList(UniformID::allStringCount(), -1)
	{

	}

	Shader::~Shader()
	{

	}

	void Shader::apply()
	{
		ShaderMgr::getInstance()->addShader(this);
		RenderPass::create(this);
	}

	void Shader::apply(const UniformID::USet& uniforms)
	{
		this->onApply(uniforms);
		this->apply();
	}

	void Shader::begin()
	{
		this->setStateOptions();
		this->bind();
	}

	void Shader::end()
	{

	}
}

