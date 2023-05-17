#include "Shader.h"
#include "../Tool/Tool.h"


#include "../Manager/ShaderManager.h"
#include "../Renderer/RenderPass.h"

namespace tezcat::Tiny
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
// 		, mViewMatrixID(0)
// 		, mModelMatrixID(0)
// 		, mProjectionMatrixID(0)
// 		, mNormalMatrixID(0)
// 		, mViewPositionID(0)
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

	void Shader::apply()
	{
		ShaderMgr::getInstance()->addShader(this);
		RenderPass::create(this);
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

